//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// 
// $Id$
//
//--------------------------------------------------------------------------

#ifndef __UQ_BASIC_PDFS_GSL_H__
#define __UQ_BASIC_PDFS_GSL_H__

#include <uqBasicPdfsBase.h>
/*! \file uqBasicPdfsGslClass.h
    \brief Class for Basic PDFs using Gsl library.
*/

/*! \class uqBasicPdfsGslClass
    \brief TODO: Base class for basic PDFs using Gsl library. 
    
    \todo This class \b will acommodate the definition of a Joint PDF using distributions
    available in the Gsl library. It will ultimately be called by uqBaseJointPdfClass and/or its
    derived classes (via m_env.basicPdfs()) during the construction of Joint PDFs.
*/
class uqBasicPdfsGslClass : public uqBasicPdfsBaseClass
{
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  uqBasicPdfsGslClass();
  
  //! Constructor.
  uqBasicPdfsGslClass(int worldRank);

  //! Destructor.
  ~uqBasicPdfsGslClass();
  //@}

  //! @name Mathematical methods
  //@{  
  //! TODO: Actual value of the Beta PDF.
  double betaPdfActualValue (double x, double alpha, double beta) const;
  
  //! TODO: Actual value of the Gamma PDF.
  double gammaPdfActualValue(double x, double a,     double b   ) const;
  //@}
protected:
};

#endif // __UQ_BASIC_PDFS_GSL_H__