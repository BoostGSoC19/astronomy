
/*=============================================================================
  Copyright 2019-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_COLUMN_HPP
#define BOOST_ASTRONOMY_IO_COLUMN_HPP

#include <string>
#include <cstddef>

#include <boost/static_assert.hpp>

/**
 * @file    column.hpp
 * @author  Pranam Lashkari
 * @details Contains definition for column structure
 */

namespace boost { namespace astronomy { namespace io {
/**
 * @brief   Used to store information about a perticular field in data array
 * @note    Refer to <a href="https://heasarc.gsfc.nasa.gov/docs/fcg/standard_dict.html">FITS_Dictionary</a>
 *          for more information on the perticular keywords
*/
struct column
{
private:
    std::size_t index_;     //Field Position associated with data array
    std::string name;       //TTYPE
    std::size_t start;      //TBCOL
    std::string format;     //TFORM
    std::string unit;       //TUNIT
    double scale;           //TSCAL
    double zero;            //TZERO
    std::string display;    //TDISP
    std::string dimension;   //TDIM
    std::string comment_;

public:
    /**
     * @brief   Constructs a standalone object of column structure
    */
    column(){}

    /**
     * @details     Constructs a column object and sets the value for
     *              starting column and encoding format for the current field value
     * @param[in]   tbcol Starting column of the current field value
     * @param[in]   tform The format in which the value of a field in encoded
    */
    column(std::size_t tbcol, std::string tform): start(tbcol), format(tform) {}

    /**
     * @brief       Constructs a column object and sets the format in which the value is encoded
     * @param[in]   tform The format in which the value of a field in encoded
    */
    column(std::string tform) : format(tform) {}

    /**
     * @brief       Sets the index/position of a perticular field
     * @param[in]   i The index/postion of the current field
    */
    void index(std::size_t i)
    {
        index_ = i;
    }

    /**
     * @brief       Gets the index/position of the current field
     * @return      The index/postion of the current field
    */
    std::size_t index() const
    {
        return index_;
    }

    /**
     * @brief       Gets the starting column of the current fields value in an ASCII table
     * @return      The starting column of current fields value if the data array is of ASCII table extention
    */
    std::size_t TBCOL() const
    {
        return start;
    }

    /**
     * @brief       Sets the starting column of the current fields value in an ASCII table
     * @param[in]   tbcol the starting column of the current fields value in an ASCII table extention
    */
    void TBCOL(std::size_t tbcol)
    {
        start = tbcol;
    }

    /**
     * @brief       Gets the name of a current field
     * @return      Return the name of a current field
    */
    std::string TTYPE() const
    {
        return name;
    }

    /**
     * @brief       Sets the name of current field
     * @param[in]   ttype Name of the current field
    */
    void TTYPE(std::string ttype)
    {
        name = ttype;
    }

    /**
     * @brief       Gets the comment associated with current field
     * @return      Returns the comment associated with current field
    */
    std::string comment() const
    {
        return comment_;
    }

    /**
     * @brief       Sets the comment associated with current field
     * @param[in]   com Comment associated with current field
    */
    void comment(std::string com)
    {
        comment_ = com;
    }

    /**
     * @brief       Gets the format in which the current field value is encoded
     * @return      Returns the format of field value
    */
    std::string TFORM() const
    {
        return format;
    }

    /**
     * @brief       Sets the format for encoding the current field's value
     * @param[in]   tform The format for encoding current field value
    */
    void TFORM(std::string tform)
    {
        format = tform;
    }

    /**
     * @details       Gets the physical unit in which the quantity in current
     *                field, after any application of TZERO and TSCALE is expressed
     * @return        Physical unit in which the quantity in current field is expressed
    */
    std::string TUNIT() const
    {
        return unit;
    }

    /**
     * @details       Sets the physical unit in which the quantity in current
     *                field, after any application of TZERO and TSCALE is expressed
     * @param[in]     tunit Physical unit in which the quantity in current field is expressed
    */
    void TUNIT(std::string tunit)
    {
        unit = tunit;
    }

    /**
     * @brief        Gets the scaling coefficient used to compute the true physical value of field
     * @details      Returns the floating point number representing the coefficient of the linear term
     *               in the equation <strong>physical_value= TZERO+TSCAL which must be used to compute
     *               the true physical value of field
     * @return       Scaling coefficient of field 
    */
    double TSCAL() const
    {
        return scale;
    }

    /**
     * @brief        Sets the scaling coefficient used to compute the true physical value of field
     * @details      Sets the floating point number representing the coefficient of the linear term
     *               in the equation <strong>physical_value= TZERO+TSCAL which must be used to compute
     *               the true physical value of field
     * @param[in]    tscal Scaling coefficient of field
    */
    void TSCAL(double tscal)
    {
        scale = tscal;
    }

    /**
     * @details      Gets the floating point number representing
     *               the true physical value corresponding to a value of zero in current field of the FITS file 
     * @return       floating point number representing true physical value corresponding to a value of zero
    */
    double TZERO() const
    {
        return zero;
    }

    /**
     * @details       Gets the floating point number representing
     *                the true physical value corresponding to a value of zero in current field of the FITS file 
     * @param[in]     tzero floating point number representing true physical value corresponding to a value of zero
    */
    void TZERO(double tzero)
    {
        zero = tzero;
    }

    /**
     * @brief         Gets a string describing the format recommended for the display of the contents of current field
     * @return        String describing the format recommended for the display of the contents of current field
    */
    std::string TDISP() const
    {
        return display;
    }

    /**
     * @brief       Sets a string describing the format recommended for the display of the contents of current field
     * @param[in]   tdisp String describing the format recommended for the display of the contents of current field   
    */
    void TDISP(std::string tdisp)
    {
        display = tdisp;
    }

    /**
     * @brief       Gets the string describing how to intrepret the contents of current field as a multidimensional array
     * @return      String describing how to intrepret the contents of current field as a multidimensional array
    */
    std::string TDIM() const
    {
        return dimension;
    }

    /**
     * @brief        Gets the string describing how to intrepret the contents of current field as a multidimensional array
     * @param[in]    tdim String describing how to intrepret the contents of current field as a multidimensional array
    */
    void TDIM(std::string tdim)
    {
        dimension = tdim;
    }

    /**
     * @brief       Allows column to acts as a polymorphic base for other class ( column_data)
    */
    virtual ~column() {}
};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_HPP
