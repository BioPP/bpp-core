# CMake script for Bio++ Core
# Authors:
#   Sylvain Gaillard
#   Julien Dutheil
#   Francois Gindraud (2017)
# Created: 17/08/2009

# File list
set (CPP_FILES
  Bpp/App/ApplicationTools.cpp
  Bpp/App/BppApplication.cpp
  Bpp/App/NumCalcApplicationTools.cpp
  Bpp/BppString.cpp
  Bpp/Exceptions.cpp
  Bpp/Graph/GlobalGraph.cpp
  Bpp/Graphics/ColorTools.cpp
  Bpp/Graphics/Fig/XFigGraphicDevice.cpp
  Bpp/Graphics/Fig/XFigLaTeXFontManager.cpp
  Bpp/Graphics/Fig/XFigPostscriptFontManager.cpp
  Bpp/Graphics/Font/Font.cpp
  Bpp/Graphics/GraphicDevice.cpp
  Bpp/Graphics/Latex/DvipsColorSet.cpp
  Bpp/Graphics/Latex/PgfGraphicDevice.cpp
  Bpp/Graphics/Molscript/MolscriptColorSet.cpp
  Bpp/Graphics/R/RColorSet.cpp
  Bpp/Graphics/Svg/SvgGraphicDevice.cpp
  Bpp/Io/BppODiscreteDistributionFormat.cpp
  Bpp/Io/BppOParametrizableFormat.cpp
  Bpp/Io/FileTools.cpp
  Bpp/Io/IoDiscreteDistributionFactory.cpp
  Bpp/Numeric/AbstractParameterAliasable.cpp
  Bpp/Numeric/AbstractParametrizable.cpp
  Bpp/Numeric/AdaptiveKernelDensityEstimation.cpp
  Bpp/Numeric/AutoParameter.cpp
  Bpp/Numeric/DataTable.cpp
  Bpp/Numeric/Function/AbstractOptimizer.cpp
  Bpp/Numeric/Function/BfgsMultiDimensions.cpp
  Bpp/Numeric/Function/BrentOneDimension.cpp
  Bpp/Numeric/Function/ConjugateGradientMultiDimensions.cpp
  Bpp/Numeric/Function/DirectionFunction.cpp
  Bpp/Numeric/Function/DownhillSimplexMethod.cpp
  Bpp/Numeric/Function/FivePointsNumericalDerivative.cpp
  Bpp/Numeric/Function/FunctionTools.cpp
  Bpp/Numeric/Function/GoldenSectionSearch.cpp
  Bpp/Numeric/Function/MetaOptimizer.cpp
  Bpp/Numeric/Function/NewtonBacktrackOneDimension.cpp
  Bpp/Numeric/Function/NewtonOneDimension.cpp
  Bpp/Numeric/Function/OneDimensionOptimizationTools.cpp
  Bpp/Numeric/Function/Operators/ComputationTree.cpp
  Bpp/Numeric/Function/OptimizationStopCondition.cpp
  Bpp/Numeric/Function/PowellMultiDimensions.cpp
  Bpp/Numeric/Function/ReparametrizationFunctionWrapper.cpp
  Bpp/Numeric/Function/SimpleMultiDimensions.cpp
  Bpp/Numeric/Function/SimpleNewtonMultiDimensions.cpp
  Bpp/Numeric/Function/ThreePointsNumericalDerivative.cpp
  Bpp/Numeric/Function/TwoPointsNumericalDerivative.cpp
  Bpp/Numeric/Hmm/AbstractHmmTransitionMatrix.cpp
  Bpp/Numeric/Hmm/AutoCorrelationTransitionMatrix.cpp
  Bpp/Numeric/Hmm/FullHmmTransitionMatrix.cpp
  Bpp/Numeric/Hmm/HmmLikelihood.cpp
  Bpp/Numeric/Hmm/LogsumHmmLikelihood.cpp
  Bpp/Numeric/Hmm/LowMemoryRescaledHmmLikelihood.cpp
  Bpp/Numeric/Hmm/RescaledHmmLikelihood.cpp
  Bpp/Numeric/NumTools.cpp
  Bpp/Numeric/Parameter.cpp
  Bpp/Numeric/ParameterExceptions.cpp
  Bpp/Numeric/ParameterList.cpp
  Bpp/Numeric/Prob/AbstractDiscreteDistribution.cpp
  Bpp/Numeric/Prob/BetaDiscreteDistribution.cpp
  Bpp/Numeric/Prob/ConstantDistribution.cpp
  Bpp/Numeric/Prob/DirichletDiscreteDistribution.cpp
  Bpp/Numeric/Prob/ExponentialDiscreteDistribution.cpp
  Bpp/Numeric/Prob/GammaDiscreteDistribution.cpp
  Bpp/Numeric/Prob/GaussianDiscreteDistribution.cpp
  Bpp/Numeric/Prob/InvariantMixedDiscreteDistribution.cpp
  Bpp/Numeric/Prob/MixtureOfDiscreteDistributions.cpp
  Bpp/Numeric/Prob/SimpleDiscreteDistribution.cpp
  Bpp/Numeric/Prob/Simplex.cpp
  Bpp/Numeric/Prob/TruncatedExponentialDiscreteDistribution.cpp
  Bpp/Numeric/Prob/UniformDiscreteDistribution.cpp
  Bpp/Numeric/Random/ContingencyTableGenerator.cpp
  Bpp/Numeric/Random/RandomTools.cpp
  Bpp/Numeric/Random/Uniform01K.cpp
  Bpp/Numeric/Random/Uniform01QD.cpp
  Bpp/Numeric/Random/Uniform01WH.cpp
  Bpp/Numeric/Stat/ContingencyTableTest.cpp
  Bpp/Numeric/Stat/Mva/CorrespondenceAnalysis.cpp 
  Bpp/Numeric/Stat/Mva/DualityDiagram.cpp 
  Bpp/Numeric/Stat/Mva/PrincipalComponentAnalysis.cpp 
  Bpp/Numeric/Stat/StatTools.cpp
  Bpp/Numeric/VectorTools.cpp
  Bpp/Text/KeyvalTools.cpp
  Bpp/Text/NestedStringTokenizer.cpp
  Bpp/Text/StringTokenizer.cpp
  Bpp/Text/TextTools.cpp
  Bpp/Utils/AttributesTools.cpp
  )

# Build the static lib
add_library (bpp-core-static STATIC ${CPP_FILES})
target_include_directories (bpp-core-static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties (bpp-core-static PROPERTIES OUTPUT_NAME bpp-core)
target_link_libraries (bpp-core-static ${LIBS})

# Build the shared lib
add_library (bpp-core-shared SHARED ${CPP_FILES})
target_include_directories (bpp-core-shared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties (bpp-core-shared
  PROPERTIES OUTPUT_NAME bpp-core
  MACOSX_RPATH 1
  VERSION ${BPPPHYL_VERSION}
  SOVERSION ${BPPPHYL_VERSION_MAJOR}
  )
target_link_libraries (bpp-core-shared ${LIBS})

# Install libs and headers
install (
  TARGETS bpp-core-static bpp-core-shared
  EXPORT bpp-core-config
  LIBRARY DESTINATION lib${LIB_SUFFIX}
  ARCHIVE DESTINATION lib${LIB_SUFFIX}
  )
install (
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  )
install (EXPORT bpp-core-config DESTINATION lib/cmake/bpp-core)

