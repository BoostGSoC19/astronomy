#ifndef BOOST_ASTRONOMY_IO_BITPIX_HPP
#define BOOST_ASTRONOMY_IO_BITPIX_HPP

namespace boost { namespace astronomy { namespace io {

//! enum used to represetn different values of bitpix in header
enum class bitpix
{
    B8, //! 8-bit unsigned integer
    B16, //! 16-bit two's complement integer
    B32, //! 32-bit two's complement integer
    _B32, //! 32-bit IEEE single precesion floating point
    _B64 //! 64-bit IEEE double precesion floating point
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_BITPIX_HPP
