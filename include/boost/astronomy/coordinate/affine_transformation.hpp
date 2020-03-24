#ifndef AFFINE_TRANSFORMATION_HPP
#define AFFINE_TRANSFORMATION_HPP
#define ROW_SIZE 3
#include <type_traits>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/units/systems/si/length.hpp>
#include <tuple>

namespace boost { namespace astronomy { namespace coordinate {


namespace bac = boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
using namespace bac;
template
<
typename elementType = double
>
struct affine_transformation
{

public:
    typedef bac::cartesian_representation<elementType,quantity<si::length>,quantity<si::length>,quantity<si::length>> cord_rep;

    elementType affine_matrix[ROW_SIZE][ROW_SIZE];
    cord_rep translation_vec;

    //default constructor no any initialization
    affine_transformation(){
        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j) this->affine_matrix[i][j] = (elementType)(i==j);
    };

    //! construct affine_transformation object with given affine_matrix
    explicit affine_transformation(elementType affine[][ROW_SIZE]){
        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j) this->affine_matrix[i][j] = affine[i][j];
    }

    //! construct affine_transformation object with given affine_matrix and translation coordinates
    affine_transformation(elementType affine[][ROW_SIZE],cord_rep const & trans){
        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j) this->affine_matrix[i][j] = affine[i][j];
        this->translation_vec = trans;
    }


    //!set affine matrix of affine_transformation object
    void set_affine_matrix(elementType affine[][ROW_SIZE]){
        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j) this->affine_matrix[i][j] = affine[i][j];
    }

    //! set affine_matrix and translation vector 
    void set_translation_vector(cord_rep const & trans){
        this->translation_vec = trans;
    }
    cord_rep
    get_transformed_representation(cord_rep const &vec){
        std::tuple<quantity<si::length>,quantity<si::length>,quantity<si::length>> x_y_z = vec.get_x_y_z();
        std::tuple<quantity<si::length>,quantity<si::length>,quantity<si::length>> tx_ty_tz = translation_vec.get_x_y_z();
        elementType xyz[ROW_SIZE];
        elementType transformed[ROW_SIZE];

        xyz[0] = get<0>(x_y_z).value();
        xyz[1] = get<1>(x_y_z).value();
        xyz[2] = get<2>(x_y_z).value();
        
        transformed[0] = get<0>(tx_ty_tz).value();
        transformed[1] = get<1>(tx_ty_tz).value();
        transformed[2] = get<2>(tx_ty_tz).value();
        



        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j)transformed[i] += affine_matrix[i][j]*xyz[j];

        return make_cartesian_representation((transformed[0])*meter,(transformed[1])*meter,(transformed[2])*meter);
    }
};
}}}



#endif