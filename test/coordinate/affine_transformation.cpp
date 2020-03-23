#define BOOST_TEST_MODULE affine_transformation_test
#include <iostream>
#include <boost/astronomy/coordinate/affine_transformation.hpp>
#include <boost/astronomy/coordinate/cartesian_representation.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


#include <boost/units/io.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>

using namespace std;
using namespace boost::astronomy::coordinate;
using namespace boost::units;
using namespace boost::units::si;
namespace bnu = boost::numeric::ublas;

namespace bac = boost::astronomy::coordinate;
BOOST_AUTO_TEST_SUITE(affine_transformation_check)

    BOOST_AUTO_TEST_CASE(transformation){
        typedef cartesian_representation<double,quantity<si::length>,quantity<si::length>,quantity<si::length>> cord_rep;
        
        //pick a point in cartesian coordiante
        cord_rep point = make_cartesian_representation(22*meter, 13*meter, 19*meter);

        //create a object of affine_transformation class
        affine_transformation<double,quantity<si::length>,quantity<si::length>,quantity<si::length>> at;

        //define affine_matrix
        matrix<double> affine(3,3);

        //initialization of affine matrix
        //here affine is
        /*
            [
                3,-1, 6,
                2, 5, 5,
                3, 8,-1 
            ]
        */
        affine(0,0) = 3;
        affine(0,1) = -1;
        affine(0,2) = 6;
        affine(1,0) = 2;
        affine(1,1) = 5;
        affine(1,2) = 5;
        affine(2,0) = 3;
        affine(2,1) = 8;
        affine(2,2) = -1;

        std::cout << affine << std::endl;

        at.set_affine_matrix(affine);

        //define translation vector
        cord_rep translation_vector(2*meter, 3*meter, 9*meter);
        
        at.set_translation_vector(translation_vector);

        cord_rep ans = at.get_transformed_representation(point);


        BOOST_CHECK_EQUAL(ans.get_x().value(),151);
        BOOST_CHECK_EQUAL(ans.get_y().value(),198);
        BOOST_CHECK_EQUAL(ans.get_z().value(),187);

    }
BOOST_AUTO_TEST_SUITE_END()
