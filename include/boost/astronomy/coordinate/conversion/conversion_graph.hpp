/*=============================================================================
Copyright 2020 Syed Ali Hasan <alihasan9922@gmail.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iostream>
#include <stack>

//Graph
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>

//Matrix
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/astronomy/coordinate/utility/utility.hpp>

//Angle
#include <boost/units/io.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

namespace bu = boost::units;
namespace bg = boost::geometry;
namespace bac = boost::astronomy::coordinate;

using namespace boost::units;
using namespace boost::units::si;
using namespace boost::astronomy::coordinate;

enum class COORDINATE_SYSTEM {
  HORIZON,
  EQUATORIAL_HA_DEC,
  EQUATORIAL_RA_DEC,
  ECLIPTIC,
  GALACTIC};

struct CoordinateData
{
  COORDINATE_SYSTEM coordinate_system;
  std::string coordinate_name;
};

struct EdgeData
{
  std::string edge_label;
  matrix<double> conv_matrix;
};

using graph_t = boost::adjacency_list<boost::vecS, boost::vecS,
    boost::directedS,
    CoordinateData,
    EdgeData>;

using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;

template
<
  typename CoordinateType = double,
  typename Angle = bu::quantity<bu::si::plane_angle, CoordinateType>
>
matrix<double> convert(const COORDINATE_SYSTEM src,
                       const COORDINATE_SYSTEM dest,
                       const Angle& phi,
                       const Angle& st,
                       const Angle& obliquity,
                       coord_sys<2, bg::cs::spherical<bg::radian>, CoordinateType> source_coordinate)
{
  bac::column_vector<double,
      quantity<bu::si::plane_angle, double>,
      double>
      col_vec(bg::get<0>(source_coordinate.get_point()) * radians, bg::get<1>(source_coordinate.get_point()) * radians);

  graph_t G;

  const int graph_size = 5;

  vertex_t vd0 = boost::add_vertex(CoordinateData{COORDINATE_SYSTEM::HORIZON,"Horizon"}, G);
  vertex_t vd1 = boost::add_vertex(CoordinateData{COORDINATE_SYSTEM::EQUATORIAL_HA_DEC,"Equatorial_HA_Dec"}, G);
  vertex_t vd2 = boost::add_vertex(CoordinateData{COORDINATE_SYSTEM::EQUATORIAL_RA_DEC,"Equatorial_RA_Dec"}, G);
  vertex_t vd3 = boost::add_vertex(CoordinateData{COORDINATE_SYSTEM::ECLIPTIC,"Ecliptic"}, G);
  vertex_t vd4 = boost::add_vertex(CoordinateData{COORDINATE_SYSTEM::GALACTIC,"Galactic"}, G);

  boost::add_edge(vd0, vd1, EdgeData{"Horizon to Equatorial HA Dec", bac::ha_dec_horizon<double, quantity<bu::degree::plane_angle>, double>(phi).get()}, G);
  boost::add_edge(vd1, vd0, EdgeData{"Equatorial HA Dec to Horizon", bac::ha_dec_horizon<double, quantity<bu::degree::plane_angle>, double>(phi).get()}, G);
  boost::add_edge(vd1, vd2, EdgeData{"Equatorial HA Dec to Equatorial RA Dec", bac::ha_dec_ra_dec<double, quantity<bu::degree::plane_angle>, double>(st).get()}, G);
  boost::add_edge(vd2, vd1, EdgeData{"Equatorial RA Dec to Equatorial HA Dec", bac::ha_dec_ra_dec<double, quantity<bu::degree::plane_angle>, double>(st).get()}, G);
  boost::add_edge(vd2, vd3, EdgeData{"Equatorial RA Dec to Ecliptic", bac::ra_dec_to_ecliptic<double, quantity<bu::degree::plane_angle>, double>(obliquity).get()}, G);
  boost::add_edge(vd3, vd2, EdgeData{"Ecliptic to Equatorial RA Dec", bac::ecliptic_to_ra_dec<double, quantity<bu::degree::plane_angle>, double>(obliquity).get()}, G);
  boost::add_edge(vd2, vd4, EdgeData{"Equatorial RA Dec to Galactic", bac::ra_dec_to_galactic<double>().get()}, G);
  boost::add_edge(vd4, vd2, EdgeData{"Galactic to Equatorial RA Dec", bac::galactic_to_ra_dec<double>().get()}, G);

  //Predecessor Array
  boost::array<int, graph_size> predecessors{0};
  predecessors[(int)src] = (int)src;

  boost::breadth_first_search(G, (int)src, boost::visitor(
      boost::make_bfs_visitor(
          boost::record_predecessors(predecessors.begin(),
                                     boost::on_tree_edge{}))));

  //Get traversed path
  std::vector<int> store_path;

  int p = (int)dest;
  while (p != (int)src)
  {
    store_path.push_back(p);
    p = predecessors[p];
  }
  store_path.push_back(p);

  matrix<double> ans = col_vec.get();

  //Matrix Multiplication
  for(auto it = store_path.rbegin(); it + 1 != store_path.rend(); ++it)
    ans = prod(G[boost::edge(*it,*(it+1),G).first].conv_matrix, ans);

  return ans;
}