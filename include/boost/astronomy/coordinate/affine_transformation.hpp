#ifndef AFFINE_TRANSFORMATION_HPP
#define AFFINE_TRANSFORMATION_HPP
#define ROW_SIZE 3
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/units/systems/si/length.hpp>

namespace boost { namespace astronomy { namespace coordinate {


namespace bac = boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
template
<
typename elementType = double
>
class affine_transformation
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
    affine_transformation(elementType affine[][ROW_SIZE]){
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

        elementType xyz[ROW_SIZE];
        elementType transformed[ROW_SIZE];

        xyz[0] = vec.get_x().value();
        xyz[1] = vec.get_y().value();
        xyz[2] = vec.get_z().value();
        
        transformed[0] = translation_vec.get_x().value();
        transformed[1] = translation_vec.get_y().value();
        transformed[2] = translation_vec.get_z().value();
        



        for(int i=0;i<ROW_SIZE;++i)
            for(int j=0;j<ROW_SIZE;++j)transformed[i] += affine_matrix[i][j]*xyz[j];

        return make_cartesian_representation((transformed[0])*meter,(transformed[1])*meter,(transformed[2])*meter);
    }
};
}}}



#endif