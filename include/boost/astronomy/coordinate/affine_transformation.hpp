#ifndef AFFINE_TRANSFORMATION_HPP
#define AFFINE_TRANSFORMATION_HPP

#include <boost/static_assert.hpp>
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
using namespace bnu;
template
<
typename elementType = double
>
struct affine_transformation
{

public:
    typedef bac::cartesian_representation<elementType,quantity<si::length>,quantity<si::length>,quantity<si::length>> cord_rep;

    bnu::matrix<elementType> affine_matrix;
    cord_rep translation_vec;

    //default constructor no any initialization
    affine_transformation(){
        int size1 = 3;
        int size2 = 3;
        this->affine_matrix.resize(size1,size2);
    };

    //! construct affine_transformation object with given affine_matrix
    explicit affine_transformation(bnu::matrix<elementType> const & affine){
        long unsigned int size1 = affine.size1();
        long unsigned int size2 = affine.size2();
        this->affine_matrix.resize(size1,size2);
        this->affine_matrix = affine;
        std::cout << this->affine_matrix << std::endl;
    }

    //! construct affine_transformation object with given affine_matrix and translation coordinates
    affine_transformation(bnu::matrix<elementType> const & affine,cord_rep const & trans){
        this->affine_matrix = affine;
        this->translation_vec = trans;
    }


    //!set affine matrix of affine_transformation object
    void set_affine_matrix(bnu::matrix<elementType> const & affine){
        long unsigned int size1 = affine.size1();
        long unsigned int size2 = affine.size2();
        this->affine_matrix.resize(size1,size2);
        this->affine_matrix = affine;
        std::cout << this->affine_matrix << std::endl;
    }

    //! set affine_matrix and translation vector 
    void set_translation_vector(cord_rep const & trans){
        this->translation_vec = trans;
    }
    cord_rep
    get_transformed_representation(cord_rep const &vec){
        std::tuple<quantity<si::length>,quantity<si::length>,quantity<si::length>> x_y_z = vec.get_x_y_z();
        std::tuple<quantity<si::length>,quantity<si::length>,quantity<si::length>> tx_ty_tz = translation_vec.get_x_y_z();
        bnu::vector <elementType> xyz(3);
        bnu::vector <elementType> txtytz(3);
        
        xyz(0) = get<0>(x_y_z).value();
        xyz(1) = get<1>(x_y_z).value();
        xyz(2) = get<2>(x_y_z).value();
        
        txtytz(0) = get<0>(tx_ty_tz).value();
        txtytz(1) = get<1>(tx_ty_tz).value();
        txtytz(2) = get<2>(tx_ty_tz).value();
        
        
        
        bnu::vector <elementType> transformed = prod(xyz,affine_matrix) + txtytz;
        return make_cartesian_representation((transformed(0))*meter,(transformed(1))*meter,(transformed(2))*meter);
    }
};
}}}



#endif