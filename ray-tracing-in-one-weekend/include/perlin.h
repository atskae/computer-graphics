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

        // Create a Mine-crafy, blocky noise
        // Returns a double between [0, 1)
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

        // Create fuzzy/smooth noise with linear interpolation
        // Trilinear interpolation is interpolation in 3D space
        double smooth_noise(const point3& p) const {
            double floor_x = floor(p.x());
            double floor_y = floor(p.y());
            double floor_z = floor(p.z());
            
            // Get the fractional part of the double
            double u = p.x() - floor_x;
            double v = p.y() - floor_y;
            double w = p.x() - floor_z;

            // Get the integer part of the double to obtain the grid coordinates
            // The grid lines are on integers
            int i = static_cast<int>(floor_x);
            int j = static_cast<int>(floor_y);
            int k = static_cast<int>(floor_z);

            // Stores the random delta (in range (0, 1]) to add from each corner of the cube
            //  (that represents one unit of the grid)
            // c[0][0][0] = bottom-left, front corner of the cube
            // c[1][0][0] = bottom-right, front corner of the cube
            // c[1][1][1] = bottom-right, back cordner of the cube
            double c[2][2][2] = {0.0};

            // Compute the random delta from each corner of the cube
            // This is not the derivative... ?
            int mask = perlin::point_count - 1;
            for (int di=0; di<2; di++) {
                for (int dj=0; dj<2; dj++) {
                    for (int dk=0; dk<2; dk++) {
                        // Generate a random index into random_float
                        // Recall that perm_* contains random integers between [0, point_count)
                        // The `& mask` ensures we are in the array bounds of perm_*
                        // `di`, `dj`, and `dk` will only be 0, then 1
                        // So this obtains the integer from the current position in perm_*,
                        //  and the next value
                        int px = this->perm_x[(i+di) & mask];
                        int py = this->perm_y[(j+dj) & mask];
                        int pj = this->perm_z[(k+dk) & mask]; 
                        int index = px ^ py ^ pj;
                        
                        // random_float contains values between [0, 1)
                        //  with array size of pount_count
                        c[di][dj][dk] = this->random_float[index];
                    }
                }
            }

            return perlin::trilinear_interpolation(c, u, v, w);
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

        // Args:
        //  c[2][2][2], the random delta between [0, 1) from each corner of the cube
        //  u, v, w are the fractional part of the x, y, and z coordinates, respectively
        // Returns:
        //  A random double between [0, 1)
        static double trilinear_interpolation(double c[2][2][2], double u, double v, double w) {
            double accum = 0.0;
            // Iterate over the deltas for the cube corners, c[2][2][2]
            for (int i=0; i<2; i++) {
                for (int j=0; j<2; j++) {
                    for (int k=0; k<2; k++) {
                        // First iteration: 1-u
                        // Second interation: u
                        double x_delta = i*u + (1-i)*(1-u);
                        double y_delta = j*v + (1-j)*(1-v);
                        double z_delta = k*w + (1-k)*(1-w);
                        double delta_product = x_delta * y_delta * z_delta;
                        
                        accum += (delta_product * c[i][j][k]);
                    }
                }
            }
            
            return accum;
        }
};

#endif // header guard
