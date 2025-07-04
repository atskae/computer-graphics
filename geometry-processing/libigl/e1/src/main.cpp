#include <iostream>
#include <sys/stat.h>

#include <igl/readOFF.h>
#include <imgui.h>
#include <viewer_proxy.h>

#include <igl/vertex_triangle_adjacency.h>
#include <igl/adjacency_list.h>
#include <igl/per_face_normals.h>
#include <igl/per_vertex_normals.h>
#include <igl/per_corner_normals.h>

using namespace std;

// Vertex array, #V x3
Eigen::MatrixXd V;
// Face array, #F x3
Eigen::MatrixXi F;
// Per-face normal array, #F x3
Eigen::MatrixXd FN;
// Per-vertex normal array, #V x3
Eigen::MatrixXd VN;
// Per-corner normal array, (3#F) x3
Eigen::MatrixXd CN;
// Vectors of indices for adjacency relations
std::vector<std::vector<int>> VF, VFi, VV;
// Integer vector of component IDs per face, #F x1
Eigen::VectorXi cid;
// Per-face color array, #F x3
Eigen::MatrixXd component_colors_per_face;

/* Imgui Custom menu options */
// Angle threshold for per-corner shading, in degrees
int per_corner_shading_threshold = 90;

void subdivide_sqrt3(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
                     Eigen::MatrixXd &Vout, Eigen::MatrixXi &Fout) {}

bool callback_key_down(ViewerProxy &viewer, unsigned char key, int modifiers) {
  if (key == '1') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    // For each vertex, finds all the triangles that are adjacent to it
    igl::vertex_triangle_adjacency(V, F, VF, VFi);
    for(int vertex_index=0; vertex_index<VF.size(); vertex_index++) {
      // Indices into faces F that are adjacent to vertex `vertex_index`
      auto adjacent_faces = VF.at(vertex_index);
      auto index_of_incidence = VFi.at(vertex_index);
      std::cout << "Vertex #" << vertex_index << std::endl; 
      //  " (" << V.row(vertex_index) << "): " << adjacent_faces.size() << " adjacent faces:" << std::endl;
      for (auto fi: adjacent_faces) {
        auto face = F.row(fi);
        std::cout << "Face " << fi << ": " << face << std::endl;
        //for (auto vi: face) {
        //  std::cout << "(" << V.row(vi) << "), ";
        //}
        //std::cout << endl;
      }
      std::cout << "Index of incidence: ";
      for (auto ii: index_of_incidence) {
        std::cout << ii << ", ";
      }
      std::cout << std::endl;
      std::cout << "---" << std::endl;
    }
  }

  if (key == '2') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    // Given a vertex, finds all adjacent vertices
    igl::adjacency_list(F, VV);
    for (int vertex_index=0; vertex_index<VV.size(); vertex_index++) {
      auto adjacent_vertices = VV.at(vertex_index);
      std::cout << "Vertex #" << vertex_index << " adjacent vertices: ";
      for (auto vi: adjacent_vertices) {
        std::cout << vi << ", ";
      }
      std::cout << std::endl;
    }
  }

  if (key == '3') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    FN.setZero(F.rows(), 3);
    std::cout << "Per-face (flat-shading)" << std::endl;
    // Flat-shading (use normals of the triangle)
    // FN is a matrix where, there are # Faces rows
    // Each row i contains the normal vector of the Face i
    igl::per_face_normals(V, F, FN);
    // Set the viewer normals.
    viewer.data().set_normals(FN);
  }

  if (key == '4') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    std::cout << "Per-vertex shading" << std::endl;
    // Per-vertex shading
    // The average normal vector (from a vertex's neighboring vertices) is computed
    // which is then used for lighting calculations
    igl::per_vertex_normals(V, F, VN);
    // Set the viewer normals.
    viewer.data().set_normals(VN);
  }

  if (key == '5') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    std::cout << "Per-corner shading with threshold: " << per_corner_shading_threshold << std::endl;
    igl::per_corner_normals(V, F, per_corner_shading_threshold, CN);
    viewer.data().set_normals(CN);
  }

  if (key == '6') {
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    component_colors_per_face.setZero(F.rows(), 3);
    // Add your code for computing per-face connected components here:
    // store the component labels in cid.

    // Compute colors for the faces based on components, storing them in
    // component_colors_per_face.

    // Set the viewer colors
    viewer.data().set_colors(component_colors_per_face);
  }

  if (key == '7') {
    Eigen::MatrixXd Vout;
    Eigen::MatrixXi Fout;
    // Fill the subdivide_sqrt3() function with your code for sqrt(3)
    // subdivision.
    subdivide_sqrt3(V, F, Vout, Fout);
    // Set up the viewer to display the new mesh
    V = Vout;
    F = Fout;
    viewer.data().clear();
    viewer.data().set_mesh(V, F);
  }

  return true;
}

bool load_mesh(ViewerProxy &viewer, string filename, Eigen::MatrixXd &V,
               Eigen::MatrixXi &F) {
  igl::readOFF(filename, V, F);
  viewer.data().clear();
  viewer.data().set_mesh(V, F);
  viewer.data().compute_normals();
  viewer.core().align_camera_center(V, F);
  return true;
}

const char *dirs_to_check[] = {"../data/",
                               "data/",
                               "assignment1/data/",
                               "../assignment1/data/",
                               "../../assignment1/data/",
                               "../../../assignment1/data/"};
std::string find_data_dir() {
  static struct stat info;
  for (int i = 0; i < sizeof(dirs_to_check) / sizeof(dirs_to_check[0]); i++) {
    if (stat(dirs_to_check[i], &info) == 0 && info.st_mode & S_IFDIR) {
      return dirs_to_check[i];
    }
  }
  throw "Could not find data directory";
}

int main(int argc, char *argv[]) {
  // Show the mesh
  ViewerProxy &viewer = ViewerProxy::get_instance();
  viewer.callback_key_down = callback_key_down;

  std::string filename;
  if (argc == 2) {
    filename = std::string(argv[1]); // Mesh provided as command line argument
  } else {
    filename = find_data_dir() + "/bunny.off";
  }

  load_mesh(viewer, filename, V, F);

  callback_key_down(viewer, '0', 0);

  // Set up the viewer menu.
  viewer.menu().callback_draw_viewer_menu = [&]() {
    // IGL's default menu (comment out to make more room for your own menu).
    viewer.menu().draw_viewer_menu();

    // Read ImGui's documentation for more information on how to add new widgets
    // to the menu.
    ImGui::Separator();
    if (ImGui::Button("I'm a button")) {
      std::cout << "and I was just clicked" << std::endl;
    }

    ImGui::SliderInt(
      "Per-corner shading threshold (degrees)",
      &per_corner_shading_threshold,
      0, 180
    );
  };

  viewer.launch();
}
