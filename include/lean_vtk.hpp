
#include <string>
#include <cassert>

#ifndef VTU_WRITER_HPP
#define VTU_WRITER_HPP

//#include <Eigen/Dense>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace polyfem {
namespace {
int index(int N, int i, int j){
    assert(N > 0);
    return i*N+j;
}
template <typename T> class VTKDataNode {

public:
  VTKDataNode() {}

  VTKDataNode(const std::string &name, const std::string &numeric_type,
              //const Eigen::MatrixXd &data = Eigen::MatrixXd(),
                const std::vector<double>& data = std::vector<double>(),
              const int n_components = 1)
      : name_(name), numeric_type_(numeric_type), data_(data),
        n_components_(n_components) {}

  inline std::vector<double>& data() { return data_; }

  void initialize(const std::string &name, const std::string &numeric_type,
                  const std::vector<double>& data, const int n_components = 1) {
    name_ = name;
    numeric_type_ = numeric_type;
    data_ = data;
    n_components_ = n_components;
  }

  void write(std::ostream &os) const {
    os << "<DataArray type=\"" << numeric_type_ << "\" Name=\"" << name_
       << "\" NumberOfComponents=\"" << n_components_
       << "\" format=\"ascii\">\n";
    //if (data_.cols() != 1) { // TODO FIXME add analogue for columns here
    if (n_components_ != 1) {
      std::cerr << "writing matrix in vtu file (check ordering of values)"
                << std::endl;
    }
    //os << data_;

        const int num_points = data_.size();
        const int stride = num_points/n_components_;
        assert(double(stride) == double(num_points)/double(n_components_));

        //for (int d = 0; d < points.rows(); ++d)
        for (int d = 0; d < num_points; ++d)
        {
            //for (int i = 0; i < points.cols(); ++i)
            for (int i = 0; i < stride; ++i)
            {
                //os << points(d, i);
                int idx = index(stride, d, i); // TODO fix
                os << data_.at(idx);
                if (i < stride- 1)
                {
                    os << " ";
                }
            }

            //if(!is_volume_)
            //    os << " 0";

            os << "\n";
        }


    os << "</DataArray>\n";
  }

  inline bool empty() const { return data_.size() <= 0; }

private:
  std::string name_;
  /// Float64/
  std::string numeric_type_;
  std::vector<double> data_;
  int n_components_;
};
} // namespace

class VTUWriter {
public:
  /*bool write_tet_mesh(const std::string &path, const std::vector<double>& points,
                      const std::vector<int>& tets);*/
    bool write_tet_mesh(const std::string &path, const int dim, const int cell_size,
        const std::vector<double>& points, const std::vector<int>& tets);

  void add_field(const std::string &name, const std::vector<double>& data, const int& dimension);
  void add_scalar_field(const std::string &name, const std::vector<double>& data);
  void add_vector_field(const std::string &name, const std::vector<double>& data, const int& dimension);

  void clear();

private:
  bool is_volume_;

  std::vector<VTKDataNode<double>> point_data_;
  std::vector<VTKDataNode<double>> cell_data_;
  std::string current_scalar_point_data_;
  std::string current_vector_point_data_;

  void write_point_data(std::ostream &os);
  void write_header(const int n_vertices, const int n_elements,
                    std::ostream &os);
  void write_footer(std::ostream &os);
  void write_points(const int num_points, const std::vector<double>& points, std::ostream &os);

  //void write_cells(const std::vector<int>& tets, std::ostream &os);
    void write_cells(const int n_vertices, const std::vector<int>& tets, std::ostream &os);
};
template<unsigned int N>
int index(int i, int j){
    return i*N+j;
}
} // namespace polyfem

#endif // VTU_WRITER_HPP

/*template <class T>
T add(T a, T b){
    return a + b;
}*/
