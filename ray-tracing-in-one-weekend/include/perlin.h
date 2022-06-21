#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

// Perlin Noise
class perlin {
    public:
        // Constructor
        perlin() {
            // Generate an array of random doubles, each of range [0, 1)
            this->random_float = new double[perlin::point_count];
            for (int i=0; i<perlin::point_count; i++) {
                this->random_float[i] = random_double();
            }

            // Generate integer Perlin permutations for each axis: x, y, z
            this->perm_x = perlin::perlin_generate_perm();
            this->perm_y = perlin::perlin_generate_perm();
            this->perm_z = perlin::perlin_generate_perm();
        }

        // Destructor (not destroyer)
        ~perlin() {
            delete[] this->random_float;
            delete[] this->perm_x;
            delete[] this->perm_y;
            delete[] this->perm_z;
        }

        double noise(const point3& p) const {
            // TODO: why multiply by 4?
            // `& mask` will return a value that is less than the point_count
            auto mask = perlin::point_count - 1;
            auto i = static_cast<int>(4*p.x()) & mask;
            auto j = static_cast<int>(4*p.y()) & mask;
            auto k = static_cast<int>(4*p.z()) & mask;

            // Generate a random-ish index using XOR
            // Each value inside of perm_* is less than point_count
            int index = this->perm_x[i] ^ this->perm_y[j] ^ this->perm_z[k];
            return this->random_float[index];
        }

    private:
        // Number of random samples to generate
        // `static` means any object of this class can access this value
        static const int point_count = 256;
        // A dynamically allocated array of floats, array of size point_count
        double* random_float = nullptr;
        // Permutations in each axis: x,y,z. Arrays of size point_count
        int* perm_x = nullptr;
        int* perm_y = nullptr;
        int* perm_z = nullptr;

        // Generate a Perlin permutation (integers with range [0, point_count] (for an axis: x, y, z)
        static int* perlin_generate_perm() {
            // Allocate an array to hold the permutation
            int* perm = new int[perlin::point_count];

            // Set each integer in the array to it's index value
            // This double colon syntax looks Rusty
            for (int i=0; i<perlin::point_count; i++) {
                perm[i] = i;
            }

            // Create the random swaps
            perlin::permute(perm, perlin::point_count);

            return perm;
        }

        // Perform semi-random swap for each integer in the array
        // Over each index, the swap range decreases from the array size to zero
        static void permute(int* perm, int perm_size) {
            // Swap each value with a random index
            // But over time, the swap index range decreases from perm_size toward zero
            for (int i=perm_size-1; i>0; i--) {
                // Random index to swap with
                int target = random_int(0, i);
                // Swap!
                int temp = perm[i];
                perm[i] = perm[target];
                perm[target] = temp;
            }
        }
};

#endif // header guard
