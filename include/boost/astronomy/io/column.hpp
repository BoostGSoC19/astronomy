#ifndef BOOST_ASTRONOMY_IO_COLUMN_HPP
#define BOOST_ASTRONOMY_IO_COLUMN_HPP

#include <string>
#include <cstddef>

#include <boost/static_assert.hpp>

#include <boost/astronomy/io/hdu.hpp>

namespace boost { namespace astronomy { namespace io {

struct hdu;

struct column
{
private:
    std::size_t index_;     //index
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

    column(){}

    column(std::size_t tbcol, std::string tform): start(tbcol), format(tform) {}

    column(std::string tform) : format(tform) {}

    void index(std::size_t i)
    {
        index_ = i;
    }

    std::size_t index() const
    {
        return index_;
    }

    std::size_t TBCOL() const
    {
        return start;
    }

    void TBCOL(std::size_t tbcol)
    {
        start = tbcol;
    }

    std::string TTYPE() const
    {
        return name;
    }

    void TTYPE(std::string ttype)
    {
        name = ttype;
    }

    std::string comment() const
    {
        return comment_;
    }

    void comment(std::string com)
    {
        comment_ = com;
    }

    std::string TFORM() const
    {
        return format;
    }

    void TFORM(std::string tform)
    {
        format = tform;
    }

    std::string TUNIT() const
    {
        return unit;
    }

    void TUNIT(std::string tunit)
    {
        unit = tunit;
    }

    double TSCAL() const
    {
        return scale;
    }

    void TSCAL(double tscal)
    {
        scale = tscal;
    }

    double TZERO() const
    {
        return zero;
    }

    void TZERO(double tzero)
    {
        zero = tzero;
    }

    std::string TDISP() const
    {
        return display;
    }

    void TDISP(std::string tdisp)
    {
        display = tdisp;
    }

    std::string TDIM() const
    {
        return dimension;
    }

    void TDIM(std::string tdim)
    {
        dimension = tdim;
    }

    friend struct hdu;
};

}}}

#endif // !BOOST_ASTRONOMY_IO_COLUMN_HPP
