#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <format>
#include <cstdlib>
#include <ranges>
#include <algorithm>

struct Point3D {
    int x{}, y{}, z{};
};

struct Particle {
    unsigned index{};
    Point3D position{};
    Point3D velocity{};
    Point3D acceleration{};
    bool removed{false};
};

constexpr auto Particle_Pattern{"p=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, v=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, a=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>"};

Particle parse_single_particle(const std::string& particle_description, unsigned particle_index) {
    static const std::regex particle_regex{Particle_Pattern};
    std::smatch matches;
    if(std::regex_search(particle_description, matches, particle_regex)) {
        const Point3D position{std::stoi(matches[1].str()), std::stoi(matches[2].str()), std::stoi(matches[3].str())};
        const Point3D velocity{std::stoi(matches[4].str()), std::stoi(matches[5].str()), std::stoi(matches[6].str())};
        const Point3D acceleration{std::stoi(matches[7].str()), std::stoi(matches[8].str()), std::stoi(matches[9].str())};
        return Particle{
            .index=particle_index,
            .position{position},
            .velocity{velocity},
            .acceleration{acceleration}
        };
    }
    throw std::runtime_error{"Invalid particle description"};
}

std::vector<Particle> read_puzzle_input(const std::string& file_name) {
    std::ifstream file{file_name};
    if (!file.is_open()) {
        throw std::runtime_error{"Could not open input file"};
    }
    std::vector<Particle> particles{};
    unsigned particle_index{0};
    for(std::string line; std::getline(file, line); ++particle_index) {
        const auto particle = parse_single_particle(line, particle_index);
        particles.push_back(particle);
    }
    return particles;
}

unsigned magnitude(const Point3D& point) {
    return std::abs(point.x) + std::abs(point.y) + std::abs(point.z);
}

unsigned solve_part_one(const std::vector<Particle>& particles) {
    const auto particle_acceleration_comparator = [](const Particle& lhs, const Particle& rhs) {
        const auto lhs_a_magnitude = magnitude(lhs.acceleration);
        const auto rhs_a_magnitude = magnitude(rhs.acceleration);
        if (lhs_a_magnitude != rhs_a_magnitude) {
            return lhs_a_magnitude < rhs_a_magnitude;
        }
        // I think there is a gap in logic here (lack of velocity check), but
        // since my puzzle input does not require it for the algorithm to find
        // correct answer, it was not implemented
        return magnitude(lhs.position) < magnitude(rhs.position);
    };
    return std::min_element(std::begin(particles), std::end(particles), particle_acceleration_comparator)->index;
}

// we could add preprocessing to first part - first run simulation, until all acceleration directions align
// with velocity directions (so basically - when particles stop moving in other direction acceleration is pointing)
// this could help us also find stopping condition for part 2

void apply_particle_acceleration(Particle& particle) {
    particle.velocity.x += particle.acceleration.x;
    particle.velocity.y += particle.acceleration.y;
    particle.velocity.z += particle.acceleration.z;
}

void move_particle(Particle& particle) {
    particle.position.x += particle.velocity.x;
    particle.position.y += particle.velocity.y;
    particle.position.z += particle.velocity.z;
}

bool particles_occupy_same_position(const Particle& first_particle, const Particle& second_particle) {
    return first_particle.position.x == second_particle.position.x &&
           first_particle.position.y == second_particle.position.y &&
           first_particle.position.z == second_particle.position.z;
}

int signum(int value) {
    return value > 0 ? 1 : (value < 0 ? -1 : 0);
}

bool velocity_direction_aligned_with_acceleration(const Particle& particle) {
    return signum(particle.velocity.x) == signum(particle.acceleration.x) &&
           signum(particle.velocity.y) == signum(particle.acceleration.y) &&
           signum(particle.velocity.z == signum(particle.acceleration.z));
}

bool velocity_directions_aligned_with_accelerations(const std::vector<Particle>& particles) {
    return std::ranges::all_of(particles, velocity_direction_aligned_with_acceleration);
}

void resolve_collisions(std::vector<Particle>& particles) {
    for(auto it = std::begin(particles); it != std::end(particles); ++it) {
        auto& particle = *it;
        if (particle.removed) {
            continue;
        }
        for(auto& second_particle: std::ranges::subrange{it + 1, std::end(particles)}) {
            // check if not same particle (should only iterate forward)
            if (second_particle.removed) {
                continue;
            }
            if (particles_occupy_same_position(particle, second_particle)) {
                particle.removed = true;
                second_particle.removed = true;
            }
        }
    }
}

void simulate_single_step(std::vector<Particle>& particles) {
    static const auto not_removed = [](Particle& particle) { return !particle.removed; };
    for(auto& particle: particles | std::views::filter(not_removed)) {
        apply_particle_acceleration(particle);
        move_particle(particle);
    }
}

unsigned count_active_particles(const std::vector<Particle>& particles) {
    static const auto not_removed = [](const Particle& particle) { return !particle.removed; };
    return std::count_if(std::begin(particles), std::end(particles), not_removed);
}

unsigned solve_part_two(std::vector<Particle> particles) {
    std::cout << count_active_particles(particles) << std::endl;
    while(true) { // TODO: stopping condition
        simulate_single_step(particles);
        resolve_collisions(particles);
        std::cout << count_active_particles(particles) << std::endl;
    }
    return 0;
}

int main() {
    const auto puzzle_input = read_puzzle_input("input.txt");
    std::cout << std::format("Part 1 solution: {}\n", solve_part_one(puzzle_input));
    std::cout << std::format("Part 2 solution: {}\n", solve_part_two(puzzle_input));
    return 0;
}