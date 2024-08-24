#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>

const double INITIAL_PREY_POPULATION = 1000.0;
const double INITIAL_PREDATOR_POPULATION = 50.0;
const double PREY_BIRTH_RATE = 0.1;
const double PREY_DEATH_RATE = 0.02;
const double PREDATOR_BIRTH_RATE = 0.001;
const double PREDATOR_DEATH_RATE = 0.05;
const int SIMULATION_STEPS = 1000;
const int TRAINING_EPISODES = 1000;
const double LEARNING_RATE = 0.01;

class ReinforcementLearningAgent {
public:
    double valueEstimates[2];  // Values for "stay" and "hunt" actions

    ReinforcementLearningAgent() {
        valueEstimates[0] = 0.0; // Initial estimates
        valueEstimates[1] = 0.0;
    }

    int chooseAction() {
        if (valueEstimates[1] > valueEstimates[0]) {
            return 1; // "hunt"
        } else {
            return 0; // "stay"
        }
    }

    void updateValueEstimates(int action, double reward) {
        valueEstimates[action] += LEARNING_RATE * (reward - valueEstimates[action]);
    }
};

class Ecosystem {
public:
    double preyPopulation;
    double predatorPopulation;

    Ecosystem(double prey, double predator) : preyPopulation(prey), predatorPopulation(predator) {}

    double calculateReward(int preyAction, int predatorAction) {
        // Calculate reward based on action and population dynamics
        // You can design a more complex reward function based on the scenario
        double reward = (preyAction == 0) ? preyPopulation : 0.0;
        reward -= (predatorAction == 1) ? predatorPopulation : 0.0;
        return reward;
    }

    void simulateStep(ReinforcementLearningAgent& preyAgent, ReinforcementLearningAgent& predatorAgent) {
        int preyAction = preyAgent.chooseAction();
        int predatorAction = predatorAgent.chooseAction();

        double reward = calculateReward(preyAction, predatorAction);

        preyAgent.updateValueEstimates(preyAction, reward);
        predatorAgent.updateValueEstimates(predatorAction, reward);

        double newPreyPopulation = preyPopulation + (PREY_BIRTH_RATE * preyPopulation) - (PREY_DEATH_RATE * preyPopulation * predatorPopulation);
        double newPredatorPopulation = predatorPopulation + (PREDATOR_BIRTH_RATE * preyPopulation * predatorPopulation) - (PREDATOR_DEATH_RATE * predatorPopulation);

        preyPopulation = std::max(newPreyPopulation, 0.0);
        predatorPopulation = std::max(newPredatorPopulation, 0.0);
    }

    void runSimulation(ReinforcementLearningAgent& preyAgent, ReinforcementLearningAgent& predatorAgent) {
        for (int step = 1; step <= SIMULATION_STEPS; ++step) {
            simulateStep(preyAgent, predatorAgent);

            if (step % (SIMULATION_STEPS / 10) == 0) {
                std::cout << "Step " << step << ": Prey population: " << preyPopulation << ", Predator population: " << predatorPopulation << std::endl;
            }
        }
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    ReinforcementLearningAgent preyAgent;
    ReinforcementLearningAgent predatorAgent;

    Ecosystem ecosystem(INITIAL_PREY_POPULATION, INITIAL_PREDATOR_POPULATION);

    // Training the agents using Q-learning (reinforcement learning algorithm)
    for (int episode = 1; episode <= TRAINING_EPISODES; ++episode) {
        Ecosystem ecosystemCopy = ecosystem; // Make a copy to reset the ecosystem for each episode

        for (int step = 1; step <= SIMULATION_STEPS; ++step) {
            ecosystemCopy.simulateStep(preyAgent, predatorAgent);
        }
    }

    // Run the simulation with trained agents
    ecosystem.runSimulation(preyAgent, predatorAgent);

    return 0;
}
