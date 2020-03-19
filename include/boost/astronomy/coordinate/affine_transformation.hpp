#ifndef AFFINE_TRANSFORMATION_HPP
#define AFFINE_TRANSFORMATION_HPP

#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <tuple>

namespace boost { namespace astronomy { namespace coordinate {

namespace bnu = boost::numeric::ublas;
namespace bac = boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;

template
<
typename elementType = double,
typename XQuantity = bu::quantity<bu::si::dimensionless, elementType>,
typename YQuantity = bu::quantity<bu::si::dimensionless, elementType>,
typename ZQuantity = bu::quantity<bu::si::dimensionless, elementType>
>
struct affine_transformation{
public:
    typedef bac::cartesian_representation<elementType,XQuantity,YQuantity,ZQuantity> cord_rep;

    bnu::matrix<elementType> affine_matrix(3,3) = bnu::identity_matrix<elementType>(3);
    cord_rep translation_vec(XQuantity(0.0),YQuantity(0.0),ZQuantity(0.0));

    //default constructor no any initialization
    affine_transformation();

    //! construct affine_transformation object with given affine_matrix
    affine_transformation(bnu::matrix<elementType> const & affine){
        this->affine_matrix = affine;
    }

    //! construct affine_transformation object with given affine_matrix and translation coordinates
    affine_transformation(bnu::matrix<elementType> const & affine,cord_rep const & trans){
        this->affine_matrix = affine;
        this->translation_vec = trans;
    }


    //!set affine matrix of affine_transformation object
    void set_affine_matrix(bnu::matrix<elementType> const & affine){
        this->affine_matrix = affine;
    }

    //! set affine_matrix and translation vector 
    void set_translation_vector(cord_rep const & trans){
        this->translation_vec = trans;
    }
    cord_rep
    get_transformed_representation(cord_repr const &vec){
        std::tuple<XQuantity,YQuantity,ZQuantity> x_y_z = vec.get_x_y_z();
        std::tuple<XQuantity,YQuantity,ZQuantity> tx_ty_tz = translation_vec.get_x_y_z();
        bnu::vector <elementType> xyz(3);
        bnu::vector <elementType> txtytz(3);
        for(unsigned i=0;i<xyz.size();++i){
            xyz(i) = get<i>(x_y_z).value();
        }
        for(unsigned i=0;i<txtytz.size();++i){
            txtytz(i) = get<i>(tx_ty_tz).value();
        }
        bnu::vector <elementType> transformed = prod(xyz,affine_matrix) + txtytz;
        return cord_rep(XQuantity(transformed(0)),YQuantity(transformed(1)),ZQuantity(transformed(2)));
    }
};
}}}



#endif