#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

// Perlin Noise
class perlin {
    public:
        // Constructor
        perlin() {
            // Generate random unit vectors at the lattice/grid points
            this->random_vectors = new vec3[perlin::point_count];
            for (int i=0; i<perlin::point_count; i++) {
                this->random_vectors[i] = unit_vector(vec3::random(-1, 1));
            }

            // Generate integer Perlin permutations for each axis: x, y, z
            this->perm_x = perlin::perlin_generate_perm();
            this->perm_y = perlin::perlin_generate_perm();
            this->perm_z = perlin::perlin_generate_perm();
        }

        // Destructor (not destroyer)
        ~perlin() {
            delete[] this->random_vectors; 
            delete[] this->perm_x;
            delete[] this->perm_y;
            delete[] this->perm_z;
        }

        // Create fuzzy/smooth noise with linear interpolation
        // Trilinear interpolation is interpolation in 3D space
        // Returns a double with range [0, 1]
        double noise(const point3& p) const {
            double floor_x = floor(p.x());
            double floor_y = floor(p.y());
            double floor_z = floor(p.z());
            
            // Get the fractional part of the double
            double u = p.x() - floor_x;
            double v = p.y() - floor_y;
            double w = p.z() - floor_z;

            // Get the integer part of the double to obtain the grid coordinates
            // The grid lines are on integers
            int i = static_cast<int>(floor_x);
            int j = static_cast<int>(floor_y);
            int k = static_cast<int>(floor_z);

            // A random vector for each corner of the cube
            // c[0][0][0] = bottom-left, front corner of the cube
            // c[1][0][0] = bottom-right, front corner of the cube
            // c[1][1][1] = bottom-right, back cordner of the cube
            vec3 c[2][2][2];

            // This ensures that the index into the permutaion (per axis) that we generate
            //  will always be in range [0, point_count)
            int mask = perlin::point_count - 1;
            
            // Iterate through each corner of the cube
            for (int di=0; di<2; di++) {
                for (int dj=0; dj<2; dj++) {
                    for (int dk=0; dk<2; dk++) {
                        // Generate a random index into random_vectors
                        // Recall that perm_* contains random integers between [0, point_count)
                        // The `& mask` ensures we are in the array bounds of perm_*
                        // `di`, `dj`, and `dk` will only be 0, then 1
                        // So this obtains the lower integer and upper integer in the grid
                        //  given a float coordinate
                        int px = this->perm_x[(i+di) & mask];
                        int py = this->perm_y[(j+dj) & mask];
                        int pj = this->perm_z[(k+dk) & mask]; 
                        int index = px ^ py ^ pj;
                        
                        // Assign a random unit vector to the cube
                        c[di][dj][dk] = this->random_vectors[index];
                    }
                }
            }

            return perlin::trilinear_interpolation(c, u, v, w);
        }

    private:
        // Number of random samples to generate
        // `static` means any object of this class can access this value
        static const int point_count = 256;
        // point_count random vectors at the grid/lattice points
        vec3* random_vectors = nullptr;
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

        // Linear interpolate the value for t, given two bounds, low and high
        static double lerp(double low, double high, double t) {
            return low * (1-t) + high * t;
            //return low + t*(high - low);
        }

        // Args:
        //  c[2][2][2], the random unit vectors for each corner of the cube, the "gradient vectors"
        //  u, v, w are the fractional part of the x, y, and z coordinates, respectively
        // Returns:
        //  A random double between [0, 1)
        static double trilinear_interpolation(vec3 c[2][2][2], double u, double v, double w) {
            
            //// Ray Tracing in One Weekend logic

            //// Hermitian Smoothing
            //double uu = -2*u*u*u + 3*u*u;
            //double vv = -2*v*v*v + 3*v*v;
            //double ww = -2*w*w*w + 3*w*w;

            //double accum = 0.0;
            //for (int i=0; i<2; i++) {
            //    for (int j=0;j<2; j++) {
            //        for (int k=0; k<2; k++) {
            //            // Not the distance vector... wait it is???
            //            vec3 weight_vector(u-i, v-j, w-k);
            //            accum += (i*uu + (1-i)*(1-uu))
            //                * (j*vv + (1-j)*(1-vv))
            //                * (k*ww + (1-k)*(1-ww))
            //                * dot_product(c[i][j][k], weight_vector);
            //        }
            //    }
            //}

            //return accum;

            // Failed attempt
            // Actual Perlin noise using gradient and distant vectors

            // Calculate the distance vectors for each corner of the cube/grid
            // The distance vector is simply the difference between the point and
            //  the random vector at the corner
            
            // The point in the grid
            vec3 p(u, v, w);
            
            // The dot product between the distance vector and the gradient vector
            double influence_values[2][2][2];
            
            // Iterate through the corners of the cube
            for (int di=0; di<2; di++) {
                for (int dj=0; dj<2; dj++) {
                    for (int dk=0; dk<2; dk++) {
                        // Compute the distance vector
                        //vec3 gradient_vector = c[di][dj][dk];
                        vec3 gradient_vector = vec3(di, dj, dk);
                        vec3 distance_vector = p - gradient_vector;
                        // Compute the dot product = "influence value"
                        influence_values[di][dj][dk] = dot_product(c[di][dj][dk], distance_vector);
                    }
                }
            }

            // Linear interpolate between the influence values and the point
            // Same idea as the Wikipedia explanation on trilinear interpolation
            // On a coordinate system where +x (left -> right), +y (front-back), +z (inside screen -> front of screen, going out)

            // Hermitian smoothing...
            //double uu = -2*u*u*u + 3*u*u;
            //double vv = -2*v*v*v + 3*v*v;
            //double ww = -2*w*w*w + 3*w*w;
            double uu = u;
            double vv = v;
            double ww = w;           

            // First interpolate left plane of the cube to the right plane (x-axis)
            double c00 = perlin::lerp(influence_values[0][0][0], influence_values[1][0][0], uu);
            double c01 = perlin::lerp(influence_values[0][1][0], influence_values[1][1][0], uu);
            double c10 = perlin::lerp(influence_values[0][0][1], influence_values[1][0][1], uu);
            double c11 = perlin::lerp(influence_values[0][1][1], influence_values[1][1][1], uu);
           
            // Interpolate up/down (y-axis)
            double c0 = perlin::lerp(c00, c01, vv);
            double c1 = perlin::lerp(c10, c11, vv);

            // Get final interpolation value by interpolating up/down (z-axis) 
            double c_final = perlin::lerp(c0, c1, ww);
            return c_final;

            //// Wikipedia explanation of trilinear interpolation

            //// Compute the difference between the coordinate value and its smaller lattice coordinate
            //// "Lattice coordinate" is the integer cooridinate of the 3D grid of cubes
            //// In our case, the grid lines are spaced by 1, so x1-x0 is always 1
            //// u, v, and w has already been computed as such, so we simply remap names to match the Wikipedia explanation...
            //double xd = u;
            //double yd = v;
            //double zd = w;

            //// Interpolate along the x-axis
            //// Interpolate from the leftmost side of the cube (4 points) to the right side...
            //// For each corner point on the left-side of the cube, interpolate with the point on
            ////  the right side of the cube as you walk along the x-axis
            //// Assumes x goes left/right, y is back/forth, z is up/down 
            //double c00 = c[0][0][0]*(1 - xd) + c[1][0][0]*xd;
            //double c01 = c[0][0][1]*(1 - xd) + c[1][0][1]*xd;
            //double c10 = c[0][1][0]*(1 - xd) + c[1][1][0]*xd;
            //double c11 = c[0][1][1]*(1 - xd) + c[1][1][1]*xd;

            //// Interpolate along the y-axis (back/forth) using the interpolated values from the x-axis
            //// Start from front-most face of the cube to the back-most face of the cube
            //double c0 = c00*(1 - yd) + c10*yd;
            //double c1 = c01*(1 - yd) + c11*yd;

            //// Interpolate along the z-axis (up/down) using the interpolated values from the y-axis
            //// Baked ziti, baked zd 笑`
            //double lerp_value = c0*(1 - zd) + c1*zd;

            //// We done! This is the final value of the point
            //// Recall the distinction of the value of the point and the position of the point
            //// We interpolated the point's value using the surrounding grid points (corners of the cube, c)
            //return lerp_value;

            // Textbook implementation with minimal explanation...
            //double accum = 0.0;
            //// Iterate over the deltas for the cube corners, c[2][2][2]
            //for (int i=0; i<2; i++) {
            //    for (int j=0; j<2; j++) {
            //        for (int k=0; k<2; k++) {
            //            // First iteration: 1-u
            //            // Second interation: u
            //            // Each delta value is [0, 1)
            //            double x_delta = i*u + (1-i)*(1-u);
            //            double y_delta = j*v + (1-j)*(1-v);
            //            double z_delta = k*w + (1-k)*(1-w);
            //            
            //            // Also [0, 1)
            //            double delta_product = x_delta * y_delta * z_delta;
            //            
            //            // c[i][j][k] is also [0, 1)
            //            accum += (delta_product * c[i][j][k]);
            //        }
            //    }
            //}
            
            //return accum;
        }
};

#endif // header guard
