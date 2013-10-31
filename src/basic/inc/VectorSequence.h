//-----------------------------------------------------------------------Bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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

#ifndef UQ_VECTOR_SEQUENCE_H
#define UQ_VECTOR_SEQUENCE_H

#undef UQ_CODE_HAS_MONITORS

#include <queso/VectorSpace.h>
#include <queso/BoxSubset.h>
#include <queso/ScalarSequence.h>
#include <queso/SequenceStatisticalOptions.h>
#include <queso/ArrayOfOneDGrids.h>
#include <queso/ArrayOfOneDTables.h>
#include <queso/2dArrayOfStuff.h>
#include <sys/time.h>
#include <fstream>

namespace QUESO {

/*! \file VectorSequence.h
 * \brief A templated class for handling vector and arrays samples
 * 
 * \class BaseVectorSequence
 * \brief Base class for handling vector and array samples (sequence of vectors or arrays).
 *
 * This class handles either vector or array  samples generated by an algorithm, as well 
 * as operations that can be carried over them, e.g., calculation of means, correlation 
 * and covariance matrices. */


template <class V, class M>
class BaseVectorSequence
{
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  BaseVectorSequence(const VectorSpace<V,M>& vectorSpace,
			    unsigned int                   subSequenceSize,
			    const std::string&             name);
  //! Destructor.
  virtual ~BaseVectorSequence();
  //@}
  
  //! @name Sequence methods
  //@{ 
  //! Size of the sub-sequence of vectors. See template specialization.
  virtual  unsigned int    subSequenceSize             () const = 0;
  
  //! Calculates the size of the unified sequence of vectors.
  /*! TODO: this procedure does not support parallel vectors yet.*/
  unsigned int             unifiedSequenceSize         () const;
  
  //! Local dimension (size) of the vector space.
  unsigned int             vectorSizeLocal             () const;
  
  //! Global dimension (size) of the vector space.
  unsigned int             vectorSizeGlobal            () const;
  
  //! Vector space; access to protected attribute VectorSpace<V,M>& m_vectorSpace.
  const    VectorSpace<V,M>& vectorSpace        () const;
  
  //! Access to protected attribute \c m_name: name of the sequence of vectors.
  /*! This method is used, for instance, to recover the name of a sequence of vector samples, 
   * such as the 'rawChain' for the raw chain of samples used in an Monte Carlo algorithm. */
  const    std::string&    name                        () const;
  
  //! Changes the name of the sequence of vectors.
  void                     setName                     (const std::string& newName);
  
  //! Reset the values and the size of the sequence of vectors.
  void                     clear                       ();
  
  //! Finds the minimum value of the sub-sequence.
  const    V&              subMinPlain                 () const;
  
  //! Finds the minimum value of the unified sequence.
  const    V&              unifiedMinPlain             () const;
  
  //! Finds the maximum value of the sub-sequence.
  const    V&              subMaxPlain                 () const;
  
  //! Finds the maximum value of the unified sequence.
  const    V&              unifiedMaxPlain             () const;
  
  //! Finds the mean value of the sub-sequence.
  const    V&              subMeanPlain                () const;
  
  //! Finds the mean value of the unified sequence.
  const    V&              unifiedMeanPlain            () const;
  
  //! Finds the median value of the sub-sequence.
  const    V&              subMedianPlain              () const;
  
  //! Finds the median value of the unified sequence.
  const    V&              unifiedMedianPlain          () const;
  
  //! Finds the variance of a sample of the sub-sequence.
  const    V&              subSampleVariancePlain      () const;
  
   //! Finds the variance of a sample of the unified sequence.
  const    V&              unifiedSampleVariancePlain  () const;
  
  //! Finds a box subset of the sub-sequence (given  by its min and max values calculated via subMinPlain and subMaxPlain).
  const    BoxSubset<V,M>&   subBoxPlain        () const;
  
  //! Finds a box subset of the unified-sequence (given  by the min and max values of the unified sequence calculated via unifiedMinPlain and unifiedMaxPlain).
  const    BoxSubset<V,M>&   unifiedBoxPlain    () const;
  
  //! Deletes all the stored vectors.
  /*! It deletes all stored vectors and assigns NULL value to the pointers: m_subMinPlain, 
   * m_unifiedMinPlain, m_subMaxPlain, m_unifiedMaxPlain, m_subMeanPlain, m_unifiedMeanPlain, 
   * m_subMedianPlain, m_unifiedMedianPlain, m_subBoxPlain, m_unifiedBoxPlain, 
   * m_subSampleVariancePlain, m_unifiedSampleVariancePlain. */
  void                     deleteStoredVectors         ();

  //! Appends the vector \c src to \c this vector.
  /*! This routine deletes all stored computed vectors */
  void                     append                      (const BaseVectorSequence<V,M>& src,
							unsigned int                    initialPos,
							unsigned int                    numPos); 
  //! Finds the positions where the maximum element occurs in the sub-sequence.
  double                   subPositionsOfMaximum       (const ScalarSequence<double>& subCorrespondingScalarValues,
							BaseVectorSequence<V,M>&      subPositionsOfMaximum);
   
  //! Finds the positions where the maximum element occurs in the unified sequence.
  double                   unifiedPositionsOfMaximum   (const ScalarSequence<double>& subCorrespondingScalarValues,
							BaseVectorSequence<V,M>&      unifiedPositionsOfMaximum);
  
  
  //! Resize the sequence. See template specialization.
  /*! This routine deletes all stored computed vectors */
  virtual  void            resizeSequence              (unsigned int newSubSequenceSize) = 0;                      
  
  //! Reset the values of the sequence. See template specialization.
  /*! This routine deletes all stored computed vectors */
  virtual  void            resetValues                 (unsigned int initialPos, unsigned int numPos) = 0;
  
  //! Erase \c numPos positions in the sequence, starting at position \c initialPos.
  /*! This routine deletes all stored computed vectors */
  virtual  void            erasePositions              (unsigned int initialPos, unsigned int numPos) = 0; 
  
  
  //! Gets the values of the sequence at position \c posId and stores them at \c vec. See template specialization.
  virtual  void            getPositionValues           (unsigned int posId,       V& vec) const = 0;
  
  //! Set the values in \c vec  at position \c posId  of the sequence. See template specialization.
  /*! This routine deletes all stored computed vectors */
  virtual  void            setPositionValues           (unsigned int posId, const V& vec) = 0;
  
  //! Sets the values of the sequence as a Gaussian distribution of mean given by \c meanVec and standard deviation by \c stdDevVec.
  /*! This routine deletes all stored computed vectors */
  void                     setGaussian                 (const V& meanVec, const V& stdDevVec);
  
  //! Sets the values of the sequence as a uniform distribution between the values given by vectors \c aVec and \c bVec.
  /*! This routine deletes all stored computed vectors */
  void                     setUniform                  (const V& aVec,    const V& bVec     ); 

  //! Finds the mean value of the sub-sequence, considering \c numPos positions starting at position \c initialPos. See template specialization.
  virtual  void           subMeanExtra                (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       V&                                       meanVec) const = 0;
								 
  //! Finds the mean value of the unified sequence of \c numPos positions starting at position \c initialPos. See template specialization.
  virtual  void           unifiedMeanExtra            (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       V&                                       unifiedMeanVec) const = 0;
  //! Finds the median value of the sub-sequence, considering \c numPos positions starting at position \c initialPos. See template specialization.
  virtual  void           subMedianExtra              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       V&                                       medianVec) const = 0;
  //! Finds the median value of the unified sequence, considering \c numPos positions starting at position \c initialPos. See template specialization.
  virtual  void           unifiedMedianExtra          (unsigned int                             initialPos,
						       unsigned int                             localNumPos,
						       V&                                       unifiedMedianVec) const = 0;
  //! Finds the sample variance of the sub-sequence, considering \c numPos positions starting at position \c initialPos and of mean \c meanVec. See template specialization.
  virtual  void           subSampleVarianceExtra      (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const V&                                 meanVec,
						       V&                                       samVec) const = 0;
  //! Finds the sample variance of the unified sequence, considering \c numPos positions starting at position \c initialPos and of mean \c meanVec. See template specialization.
  virtual  void           unifiedSampleVarianceExtra  (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const V&                                 unifiedMeanVec,
						       V&                                       unifiedSamVec) const = 0;
  //! Finds the population variance of the sub-sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec. See template specialization.
   virtual  void           subPopulationVariance       (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const V&                                 meanVec,
						       V&                                       popVec) const = 0;
    //! Finds the population variance of the unified-sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec. See template specialization.
  virtual  void           unifiedPopulationVariance   (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const V&                                 unifiedMeanVec,
						       V&                                       unifiedPopVec) const = 0;

  //! Calculates the autocovariance. See template specialization.
  /*! The autocovariance is the covariance of a variable with itself at some other time. It is 
   * calculated over a sequence of vectors with initial position \c initialPos, considering 
   * \c numPos positions, a lag of \c lag, with mean given by \c meanVec. The results are saved
   * in the output vector \c covVec/   */						       
  virtual  void           autoCovariance              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const V&                                 meanVec,
						       unsigned int                             lag,
						       V&                                       covVec) const = 0;
  //! Calculates autocorrelation via definition. See template specialization.
  virtual  void           autoCorrViaDef              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       unsigned int                             lag,
						       V&                                       corrVec) const = 0;
  //! Calculates autocorrelation via Fast Fourier transforms (FFT). See template specialization.
  virtual  void           autoCorrViaFft              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const std::vector<unsigned int>&         lags,
						       std::vector<V*>&                         corrVecs) const = 0;
  //! Calculates autocorrelation via Fast Fourier transforms (FFT). See template specialization.
  virtual  void           autoCorrViaFft              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       unsigned int                             numSum,
						       V&                                       autoCorrsSumVec) const = 0;

						       
  //! Finds the minimum and the maximum values of the sub-sequence, considering \c numPos positions starting at position \c initialPos. See template specialization. 
  virtual  void           subMinMaxExtra              (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       V&                                       minVec,
						       V&                                       maxVec) const = 0;
  //! Finds the minimum and the maximum values of the unified sequence, considering \c numPos positions starting at position \c initialPos. See template specialization. 
  virtual  void           unifiedMinMaxExtra          (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       V&                                       unifiedMinVec,
						       V&                                       unifiedMaxVec) const = 0;
  //! Calculates the histogram of the sub-sequence. See template specialization.
  /*! The IQR is a robust estimate of the spread of the data, since changes in the upper and
  * lower 25% of the data do not affect it. If there are outliers in the data, then the IQR 
  * is more representative than the standard deviation as an estimate of the spread of the 
  * body of the data. The IQR is less efficient than the standard deviation as an estimate 
  * of the spread when the data is all from the normal distribution. (from Matlab)*/
  virtual  void           subHistogram                (unsigned int                             initialPos,
						       const V&                                 minVec,
						       const V&                                 maxVec,
						       std::vector<V*>&                         centersForAllBins,
						       std::vector<V*>&                         quanttsForAllBins) const = 0;
  //! Calculates the histogram of the unified sequence. See template specialization.
  virtual  void           unifiedHistogram            (unsigned int                             initialPos,
						       const V&                                 unifiedMinVec,
						       const V&                                 unifiedMaxVec,
						       std::vector<V*>&                         unifiedCentersForAllBins,
						       std::vector<V*>&                         unifiedQuanttsForAllBins) const = 0;

 //! Returns the interquartile range of the values in the sub-sequence. See template specialization.
  virtual  void           subInterQuantileRange       (unsigned int                             initialPos,
						       V&                                       iqrVec) const = 0;
 //! Returns the interquartile range of the values in the unified sequence. See template specialization.
  virtual  void           unifiedInterQuantileRange   (unsigned int                             initialPos,
						       V&                                       unifiedIqrVec) const = 0;

 //! Selects the scales (bandwidth, \c scaleVec) for the kernel density estimation, considering only the sub-sequence. See template specialization.
 /*! The bandwidth of the kernel is a free parameter which exhibits a strong influence on the 
  * resulting estimate. Silverman (1986) suggests the following normal-based estimates:
  *   S1 = 1.06 × (standard deviation) × n^{-1/5}
  *   S2 = 0.79 × (\c iqrVec) × n^{-1/5}, where \c iqrVec is the interquartile range
  *   \c scaleVec = 0.90 × minimum(standard deviation, \c iqrVec /1.34) × n^{-1/5}.
  * These estimates are popular due to their simplicity, and are used in QUESO with the adaptation
  * of the exponent oven the sample size n (-1/5) with -1/(4 + \c kdeDimension) where \c kdeDimension
  * is the  KDE dimension.  */						       
  virtual  void           subScalesForKde             (unsigned int                             initialPos,
						       const V&                                 iqrVec,
						       unsigned int                             kdeDimension,
						       V&                                       scaleVec) const = 0;
  //! Selects the scales (bandwidth) for the kernel density estimation, considering the unified sequence. See template specialization.
  virtual  void           unifiedScalesForKde         (unsigned int                             initialPos,
						       const V&                                 unifiedIqrVec,
						       unsigned int                             kdeDimension,
						       V&                                       unifiedScaleVec) const = 0;
  //! Gaussian kernel for the KDE estimate of the sub-sequence. See template specialization.
  /*! Computes a probability density estimate of the sample in \c this sub-sequence, starting
   * at position \c initialPos. \c densityVecs is the vector of density values evaluated at 
   * the points in \c evaluationParamVecs. The estimate is based on Gaussian (normal) kernel 
   * function, using a window parameter (\c scaleVec).*/						       
  virtual  void           subGaussian1dKde            (unsigned int                             initialPos,
						       const V&                                 scaleVec,
						       const std::vector<V*>&                   evaluationParamVecs,
						       std::vector<V*>&                         densityVecs) const = 0;
  //! Gaussian kernel for the KDE estimate of the unified sequence. See template specialization.
  virtual  void           unifiedGaussian1dKde        (unsigned int                             initialPos,
						       const V&                                 unifiedScaleVec,
						       const std::vector<V*>&                   unifiedEvaluationParamVecs,
						       std::vector<V*>&                         unifiedDensityVecs) const = 0;
  //! Writes info of the sub-sequence to a file. See template specialization.
  virtual  void           subWriteContents            (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       const std::string&                       fileName,
						       const std::string&                       fileType,
						       const std::set<unsigned int>&            allowedSubEnvIds) const = 0;
  //! Writes info of the sub-sequence to a file. See template specialization.
  virtual  void           subWriteContents            (unsigned int                             initialPos,
						       unsigned int                             numPos,
						       std::ofstream&                           ofsvar,
						       const std::string&                       fileType) const = 0;
  //! Writes info of the unified sequence to a file. See template specialization.
  virtual  void           unifiedWriteContents        (const std::string&                       fileName,
						       const std::string&                       fileType) const = 0;
  //! Reads info of the unified sequence from a file. See template specialization.
  virtual  void           unifiedReadContents         (const std::string&                       fileName,
						       const std::string&                       fileType,
						       const unsigned int                       subSequenceSize) = 0;
  //! Select positions in the sequence of vectors. See template specialization.
  virtual  void           select                      (const std::vector<unsigned int>&         idsOfUniquePositions) = 0;
  
  //! Filters positions in the sequence of vectors, starting at \c initialPos, and with spacing given by \c spacing. See template specialization. 
  virtual  void           filter                      (unsigned int                             initialPos,
                                                       unsigned int                             spacing) = 0;
  //! Computes the filtering parameters \c spacing for the sequence of vectors.
  void                    computeFilterParams         (std::ofstream*                           passedOfs,
						       unsigned int   &                         initialPos,
						       unsigned int   &                         spacing);
  //! Estimates convergence rate using  Brooks & Gelman method. See template specialization.
  virtual  double         estimateConvBrooksGelman    (unsigned int                             initialPos,
						       unsigned int                             numPos) const = 0;
  //! Extracts a sequence of scalars of size \c numPos, starting at position \c initialPos, given spacing \c spacing. See template specialization.
  virtual  void           extractScalarSeq            (unsigned int                             initialPos,
						       unsigned int                             spacing,
						       unsigned int                             numPos,
						       unsigned int                             paramId,
						       ScalarSequence<double>&           scalarSeq) const = 0;
  
#ifdef QUESO_COMPUTES_EXTRA_POST_PROCESSING_STATISTICS
  virtual  void           subUniformlySampledCdf      (const V&                       numEvaluationPointsVec,
						       ArrayOfOneDGrids <V,M>& cdfGrids,
						       ArrayOfOneDTables<V,M>& cdfValues) const = 0;
  virtual  void           unifiedUniformlySampledCdf  (const V&                       numEvaluationPointsVec,
						       ArrayOfOneDGrids <V,M>& unifiedCdfGrids,
						       ArrayOfOneDTables<V,M>& unifieddfValues) const = 0;
  virtual  void           bmm                         (unsigned int                             initialPos,
						       unsigned int                             batchLength,
						       V&                                       bmmVec) const = 0;
  virtual  void           fftForward                  (unsigned int                             initialPos,
						       unsigned int                             fftSize,
						       unsigned int                             paramId,
						       std::vector<std::complex<double> >&      fftResult) const = 0;
//virtual  void           fftInverse                  (unsigned int                             fftSize) = 0;
  virtual  void           psd                         (unsigned int                             initialPos,
						       unsigned int                             numBlocks,
						       double                                   hopSizeRatio,
						       unsigned int                             paramId,
						       std::vector<double>&                     psdResult) const = 0;
  virtual  void           psdAtZero                   (unsigned int                             initialPos,
						       unsigned int                             numBlocks,
						       double                                   hopSizeRatio,
						       V&                                       psdVec) const = 0;
  virtual  void           geweke                      (unsigned int                             initialPos,
						       double                                   ratioNa,
						       double                                   ratioNb,
						       V&                                       gewVec) const = 0;
  virtual  void           meanStacc                   (unsigned int                             initialPos,
						       V&                                       meanStaccVec) const = 0;
  virtual  void           subCdfStacc                 (unsigned int                             initialPos,
						       const std::vector<V*>&                   evalPositionsVecs,                                                                 std::vector<V*>&                         cdfStaccVecs) const = 0;
#endif //QUESO_COMPUTES_EXTRA_POST_PROCESSING_STATISTICS
						       
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
  void                    computeHistCdfstaccKde      (const SequenceStatisticalOptions& statisticalOptions,
						       std::ofstream*                           passedOfs);

  void                    computeStatistics           (const SequenceStatisticalOptions& statisticalOptions,
						       std::ofstream*                           passedOfs);
#endif //QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
	   
#ifdef UQ_ALSO_COMPUTE_MDFS_WITHOUT_KDE
  virtual  void           subUniformlySampledMdf      (const V&                       numEvaluationPointsVec,
						       ArrayOfOneDGrids <V,M>& mdfGrids,
						       ArrayOfOneDTables<V,M>& mdfValues) const = 0;
#endif //UQ_ALSO_COMPUTE_MDFS_WITHOUT_KDE
						       
  //@}

protected:
  //! Copies vector sequence \c src to \c this.
  /*! This routine deletes all stored computed vectors */
  void           copy                        (const BaseVectorSequence<V,M>&    src); 
  
  //! Extracts the raw data. See template specialization.	    
  virtual  void           extractRawData              (unsigned int                             initialPos,
                                                       unsigned int                             spacing,
                                                       unsigned int                             numPos,
                                                       unsigned int                             paramId,
                                                       std::vector<double>&                     rawData) const = 0;

  const BaseEnvironment&  m_env;
  const VectorSpace<V,M>& m_vectorSpace;
  std::string                    m_name;

  mutable Fft<double>*    m_fftObj;
  mutable V*                     m_subMinPlain;
  mutable V*                     m_unifiedMinPlain;
  mutable V*                     m_subMaxPlain;
  mutable V*                     m_unifiedMaxPlain;
  mutable V*                     m_subMeanPlain;
  mutable V*                     m_unifiedMeanPlain;
  mutable V*                     m_subMedianPlain;
  mutable V*                     m_unifiedMedianPlain;
  mutable V*                     m_subSampleVariancePlain;
  mutable V*                     m_unifiedSampleVariancePlain;
  mutable BoxSubset<V,M>* m_subBoxPlain;
  mutable BoxSubset<V,M>* m_unifiedBoxPlain;
  
	   
#ifdef UQ_CODE_HAS_MONITORS
  virtual  void           subMeanMonitorAlloc         (unsigned int numberOfMonitorPositions) = 0;
  virtual  void           subMeanInter0MonitorAlloc   (unsigned int numberOfMonitorPositions) = 0;
  virtual  void           unifiedMeanMonitorAlloc     (unsigned int numberOfMonitorPositions) = 0;

  virtual  void           subMeanMonitorRun           (unsigned int monitorPosition,
						       V&           subMeanVec,
						       V&           subMeanCltStd) = 0;
  virtual  void           subMeanInter0MonitorRun     (unsigned int monitorPosition,
						       V&           subMeanInter0Mean,
						       V&           subMeanInter0Clt95,
						       V&           subMeanInter0Empirical90,
						       V&           subMeanInter0Min,
						       V&           subMeanInter0Max) = 0;
  virtual  void           unifiedMeanMonitorRun       (unsigned int monitorPosition,
						       V&           unifiedMeanVec,
						       V&           unifiedMeanCltStd) = 0;

  virtual  void           subMeanMonitorStore         (unsigned int i,
						       unsigned int monitorPosition,
						       const V&     subMeanVec,
						       const V&     subMeanCltStd) = 0;
  virtual  void           subMeanInter0MonitorStore   (unsigned int i,
						       unsigned int monitorPosition,
						       const V&     subMeanInter0Mean,
						       const V&     subMeanInter0Clt95,
						       const V&     subMeanInter0Empirical90,
						       const V&     subMeanInter0Min,
						       const V&     subMeanInter0Max) = 0;
  virtual  void           unifiedMeanMonitorStore     (unsigned int i,
						       unsigned int monitorPosition,
						       V&           unifiedMeanVec,
						       V&           unifiedMeanCltStd) = 0;

  virtual  void           subMeanMonitorWrite         (std::ofstream& ofs) = 0;
  virtual  void           subMeanInter0MonitorWrite   (std::ofstream& ofs) = 0;
  virtual  void           unifiedMeanMonitorWrite     (std::ofstream& ofs) = 0;

  virtual  void           subMeanMonitorFree          () = 0;
  virtual  void           subMeanInter0MonitorFree    () = 0;
  virtual  void           unifiedMeanMonitorFree      () = 0;

           void           computeMeanEvolution        (const SequenceStatisticalOptions& statisticalOptions,
						       std::ofstream*                           passedOfs);
#endif
	   
#ifdef QUESO_COMPUTES_EXTRA_POST_PROCESSING_STATISTICS
           void           computeBMM                  (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
           void           computeFFT                  (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
           void           computePSD                  (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
           void           computePSDAtZero            (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
           void           computeGeweke               (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
           void           computeMeanStacc            (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       std::ofstream*                           passedOfs);
#endif
	   
#ifdef QUESO_USES_SEQUENCE_STATISTICAL_OPTIONS
           void           computeMeanVars             (const SequenceStatisticalOptions& statisticalOptions,
                                                       std::ofstream*                           passedOfs,
                                                       V*                                       subMeanPtr,
                                                       V*                                       subMedianPtr,
                                                       V*                                       subSampleVarPtr,
                                                       V*                                       subPopulVarPtr);
           void           computeAutoCorrViaDef       (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       const std::vector<unsigned int>&         lagsForCorrs,
                                                       std::ofstream*                           passedOfs);
           void           computeAutoCorrViaFFT       (const SequenceStatisticalOptions& statisticalOptions,
                                                       const std::vector<unsigned int>&         initialPosForStatistics,
                                                       const std::vector<unsigned int>&         lagsForCorrs,
                                                       std::ofstream*                           passedOfs);
           void           computeCovCorrMatrices      (const SequenceStatisticalOptions& statisticalOptions,
                                                       std::ofstream*                           passedOfs);
#endif
  
};

// --------------------------------------------------
// Additional methods --------------------------------
// (outside class declaration) ----------------------
//---------------------------------------------------
template <class P_V, class P_M, class Q_V, class Q_M>
void
ComputeCovCorrMatricesBetweenVectorSequences(
  const BaseVectorSequence<P_V,P_M>& subPSeq,
  const BaseVectorSequence<Q_V,Q_M>& subQSeq,
        unsigned int                        subNumSamples,
        P_M&                                pqCovMatrix,
        P_M&                                pqCorrMatrix);

}  // End namespace QUESO

#endif // UQ_VECTOR_SEQUENCE_H
