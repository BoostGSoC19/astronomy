
/*=============================================================================
  Copyright 2018-2020 Pranam Lashkari < lpranam - plashkari628@gmail.com >
  Copyright 2019-2020 Sarthak Singhal < sarthak2007 - singhalsarthak2007@gmail.com >

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_FITS_HPP
#define BOOST_ASTRONOMY_IO_FITS_HPP

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <boost/astronomy/io/primary_hdu.hpp>
#include <boost/astronomy/io/extension_hdu.hpp>
#include <boost/astronomy/io/image_extension.hpp>
#include <boost/astronomy/io/ascii_table.hpp>
#include <boost/astronomy/exception/fits_exception.hpp>

/**
 * @file    fits.hpp
 * @author  Pranam Lashkari
 * @author  Sarthak Singhal
 * @details
 */

namespace boost { namespace astronomy { namespace io {

/**
 * @brief   API through which the user can access and manage the data of FITS file
 * @details This structure includes the utility methods for access and manipulation of FITS data
 *          Currently it only supports reading primary_hdu and image_extention and ascii table
 * @author  Pranam Lashkari
 * @author  Sarthak Singhal
 * @todo    Provide a convinient interface to the user for working with FITS files. As of now users
            can only read fits files and not access or manipulate it.This API requires a substantial
 *          amount of change.
 */
struct fits
{
protected:
    std::fstream fits_file; //!FITS file to be processed
    std::vector<std::shared_ptr<hdu>> hdu_; //!Stores all the HDU in file

public:
    /**
     * @brief   Default constructor to create a standalone object of FITS structure
    */
    fits() {}

    /**
     * @brief       Constructs a FITS object based on the given file
     * @details     This function initializes the fits object based on file_path
     *              and as of now reads the primary hdu as well
     * @param[in]   file_path File path where the FITS file resides
    */
    fits
    (
        std::string file_path,
        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary
    )
    {
        fits_file.open(file_path, std::ios_base::in | std::ios_base::binary | mode);
        read_primary_hdu();
        //read_extensions();
    }

    /**
     * @brief       Reads the primary Header Data Unit From FITS file
     * @details     This method reads the primary header information along with data of an HDU
     * @see         primary_hdu.hpp
    */
    void read_primary_hdu()
    {
        hdu_.emplace_back(std::make_shared<hdu>(fits_file));
                    
        switch (hdu_[0]->value_of<int>(std::string("BITPIX")))
        {
        case 8:
            hdu_[0] = std::make_shared<primary_hdu<bitpix::B8>>(fits_file, *hdu_[0]);
            break;
        case 16:
            hdu_[0] = std::make_shared<primary_hdu<bitpix::B16>>(fits_file, *hdu_[0]);
            break;
        case 32:
            hdu_[0] = std::make_shared<primary_hdu<bitpix::B32>>(fits_file, *hdu_[0]);
            break;
        case -32:
            hdu_[0] = std::make_shared<primary_hdu<bitpix::_B32>>(fits_file, *hdu_[0]);
            break;
        case -64:
            hdu_[0] = std::make_shared<primary_hdu<bitpix::_B64>>(fits_file, *hdu_[0]);
            break;
        default:
            throw fits_exception();
        }
    }

    /**
     * @brief Reads all the extentions HDU information from a FITS file
     * @todo  As of now only the image extention, ascii table are being read
     *        support for binary table needs to be added as well
    */
    void read_extensions()
    {
        //if no extension then return
        if (!hdu_[0]->value_of<bool>("EXTEND"))
        {
            return;
        }

        while (!fits_file.eof())
        {
            //this statement allows up to read all the cards stored
            //It gives us the benefit of knowing which kind of data we need to store
            hdu_.emplace_back(std::make_shared<hdu>(fits_file));

            if (hdu_.back()->value_of<std::string>("XTENSION") == "'IMAGE   '")
            {
                switch (hdu_.back()->value_of<int>(std::string("BITPIX")))
                {
                case 8:
                    hdu_.back() =
                        std::make_shared<image_extension<bitpix::B8>>(fits_file, *hdu_.back());
                    break;
                case 16:
                    hdu_.back() =
                        std::make_shared<image_extension<bitpix::B16>>(fits_file, *hdu_.back());
                    break;
                case 32:
                    hdu_.back() =
                        std::make_shared<image_extension<bitpix::B32>>(fits_file, *hdu_.back());
                    break;
                case -32:
                    hdu_.back() =
                        std::make_shared<image_extension<bitpix::_B32>>(fits_file, *hdu_.back());
                    break;
                case -64:
                    hdu_.back() =
                        std::make_shared<image_extension<bitpix::_B64>>(fits_file, *hdu_.back());
                    break;
                default:
                    throw fits_exception();
                    break;
                }
            }
            else if (hdu_.back()->value_of<std::string>("XTENSION") == "'TABLE   '")
            {
                hdu_.back() = std::make_shared<ascii_table>(fits_file, *hdu_.back());
            }
                        
        }
    }
};

}}} //namespace boost::astronomy::io

#endif // !BOOST_ASTRONOMY_IO_FITS_HPP

