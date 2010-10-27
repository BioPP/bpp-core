# CMake script for Bio++ Core
# Author: Sylvain Gaillard and Julien Dutheil
# Created: 17/08/2009

# File list
set(CPP_FILES
  Bpp/Exceptions.cpp
  Bpp/BppString.cpp
  Bpp/App/ApplicationTools.cpp
  Bpp/App/BppApplication.cpp
  Bpp/App/NumCalcApplicationTools.cpp
  Bpp/Io/FileTools.cpp
  Bpp/Utils/AttributesTools.cpp
  Bpp/Text/KeyvalTools.cpp
  Bpp/Text/NestedStringTokenizer.cpp
  Bpp/Text/StringTokenizer.cpp
  Bpp/Text/TextTools.cpp
  Bpp/Graphics/ColorTools.cpp
  Bpp/Graphics/GraphicDevice.cpp
  Bpp/Graphics/Fig/XFigGraphicDevice.cpp
  Bpp/Graphics/Fig/XFigLaTeXFontManager.cpp
  Bpp/Graphics/Fig/XFigPostscriptFontManager.cpp
  Bpp/Graphics/Font/Font.cpp
  Bpp/Graphics/Latex/DvipsColorSet.cpp
  Bpp/Graphics/Latex/PgfGraphicDevice.cpp
  Bpp/Graphics/Molscript/MolscriptColorSet.cpp
  Bpp/Graphics/Svg/SvgGraphicDevice.cpp
  Bpp/Numeric/NumTools.cpp
  Bpp/Numeric/Parameter.cpp
  Bpp/Numeric/ParameterExceptions.cpp
  Bpp/Numeric/ParameterList.cpp
  Bpp/Numeric/RandomTools.cpp
  Bpp/Numeric/Uniform01K.cpp
  Bpp/Numeric/AbstractParameterAliasable.cpp
  Bpp/Numeric/Uniform01QD.cpp
  Bpp/Numeric/AbstractParametrizable.cpp
  Bpp/Numeric/Uniform01WH.cpp
  Bpp/Numeric/AdaptiveKernelDensityEstimation.cpp
  Bpp/Numeric/VectorTools.cpp
  Bpp/Numeric/AutoParameter.cpp
  Bpp/Numeric/DataTable.cpp
  Bpp/Numeric/Domain.cpp
  Bpp/Numeric/IntervalData.cpp
  Bpp/Numeric/NumConstants.cpp
  Bpp/Numeric/Function/AbstractOptimizer.cpp
  Bpp/Numeric/Function/BrentOneDimension.cpp
  Bpp/Numeric/Function/ConjugateGradientMultiDimensions.cpp
  Bpp/Numeric/Function/DirectionFunction.cpp
  Bpp/Numeric/Function/DownhillSimplexMethod.cpp
  Bpp/Numeric/Function/FivePointsNumericalDerivative.cpp
  Bpp/Numeric/Function/FunctionTools.cpp
  Bpp/Numeric/Function/GoldenSectionSearch.cpp
  Bpp/Numeric/Function/MetaOptimizer.cpp
  Bpp/Numeric/Function/NewtonOneDimension.cpp
  Bpp/Numeric/Function/OneDimensionOptimizationTools.cpp
  Bpp/Numeric/Function/OptimizationStopCondition.cpp
  Bpp/Numeric/Function/PowellMultiDimensions.cpp
  Bpp/Numeric/Function/ReparametrizationFunctionWrapper.cpp
  Bpp/Numeric/Function/SimpleMultiDimensions.cpp
  Bpp/Numeric/Function/SimpleNewtonMultiDimensions.cpp
  Bpp/Numeric/Function/ThreePointsNumericalDerivative.cpp
  Bpp/Numeric/Function/TwoPointsNumericalDerivative.cpp
  Bpp/Numeric/Hmm/LogsumHmmLikelihood.cpp
  Bpp/Numeric/Hmm/LowMemoryRescaledHmmLikelihood.cpp
  Bpp/Numeric/Hmm/RescaledHmmLikelihood.cpp
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
  Bpp/Numeric/Prob/TruncatedExponentialDiscreteDistribution.cpp
  Bpp/Numeric/Prob/UniformDiscreteDistribution.cpp
  )

set(H_FILES
  Bpp/BppString.h
  Bpp/BppVector.h
  Bpp/Clonable.h
  Bpp/Exceptions.h
  Bpp/App/ApplicationTools.h
  Bpp/App/BppApplication.h
  Bpp/App/NumCalcApplicationTools.h
  Bpp/Graphics/AbstractGraphicDevice.h
  Bpp/Graphics/ColorManager.h
  Bpp/Graphics/ColorSet.h
  Bpp/Graphics/ColorTools.h
  Bpp/Graphics/DefaultColorSet.h
  Bpp/Graphics/GraphicDevice.h
  Bpp/Graphics/Point2D.h
  Bpp/Graphics/Point2DTools.h
  Bpp/Graphics/RgbColor.h
  Bpp/Io/FileTools.h
  Bpp/Io/IoFormat.h
  Bpp/Io/OutputStream.h
  Bpp/Numeric/AbstractParameterAliasable.h
  Bpp/Numeric/AbstractParametrizable.h
  Bpp/Numeric/AdaptiveKernelDensityEstimation.h
  Bpp/Numeric/AutoParameter.h
  Bpp/Numeric/Constraints.h
  Bpp/Numeric/DataTableExceptions.h
  Bpp/Numeric/DataTable.h
  Bpp/Numeric/Domain.h
  Bpp/Numeric/IntervalData.h
  Bpp/Numeric/Number.h
  Bpp/Numeric/NumConstants.h
  Bpp/Numeric/NumTools.h
  Bpp/Numeric/ParameterAliasable.h
  Bpp/Numeric/ParameterExceptions.h
  Bpp/Numeric/Parameter.h
  Bpp/Numeric/ParameterList.h
  Bpp/Numeric/Parametrizable.h
  Bpp/Numeric/RandomFactory.h
  Bpp/Numeric/RandomTools.h
  Bpp/Numeric/TransformedParameter.h
  Bpp/Numeric/Uniform01K.h
  Bpp/Numeric/Uniform01QD.h
  Bpp/Numeric/Uniform01WH.h
  Bpp/Numeric/VectorExceptions.h
  Bpp/Numeric/VectorTools.h
  Bpp/Text/KeyvalTools.h
  Bpp/Text/NestedStringTokenizer.h
  Bpp/Text/StringTokenizer.h
  Bpp/Text/TextTools.h
  Bpp/Utils/AttributesTools.h
  Bpp/Utils/MapTools.h
  Bpp/Graphics/Fig/XFigGraphicDevice.h
  Bpp/Graphics/Fig/XFigLaTeXFontManager.h
  Bpp/Graphics/Fig/XFigPostscriptFontManager.h
  Bpp/Graphics/Font/Font.h
  Bpp/Graphics/Font/FontManager.h
  Bpp/Graphics/Latex/DvipsColorSet.h
  Bpp/Graphics/Latex/PgfGraphicDevice.h
  Bpp/Graphics/Molscript/MolscriptColorSet.h
  Bpp/Graphics/R/RColorSet.h
  Bpp/Graphics/Svg/SvgGraphicDevice.h
  Bpp/Numeric/Function/AbstractNumericalDerivative.h
  Bpp/Numeric/Function/AbstractOptimizer.h
  Bpp/Numeric/Function/BrentOneDimension.h
  Bpp/Numeric/Function/ConjugateGradientMultiDimensions.h
  Bpp/Numeric/Function/DirectionFunction.h
  Bpp/Numeric/Function/DownhillSimplexMethod.h
  Bpp/Numeric/Function/FivePointsNumericalDerivative.h
  Bpp/Numeric/Function/Functions.h
  Bpp/Numeric/Function/FunctionTools.h
  Bpp/Numeric/Function/GoldenSectionSearch.h
  Bpp/Numeric/Function/MetaOptimizer.h
  Bpp/Numeric/Function/NewtonOneDimension.h
  Bpp/Numeric/Function/OneDimensionOptimizationTools.h
  Bpp/Numeric/Function/OptimizationStopCondition.h
  Bpp/Numeric/Function/Optimizer.h
  Bpp/Numeric/Function/PowellMultiDimensions.h
  Bpp/Numeric/Function/ReparametrizationFunctionWrapper.h
  Bpp/Numeric/Function/SimpleMultiDimensions.h
  Bpp/Numeric/Function/SimpleNewtonMultiDimensions.h
  Bpp/Numeric/Function/ThreePointsNumericalDerivative.h
  Bpp/Numeric/Function/TwoPointsNumericalDerivative.h
  Bpp/Numeric/Hmm/HmmEmissionProbabilities.h
  Bpp/Numeric/Hmm/HmmExceptions.h
  Bpp/Numeric/Hmm/HmmLikelihood.h
  Bpp/Numeric/Hmm/HmmStateAlphabet.h
  Bpp/Numeric/Hmm/HmmTransitionMatrix.h
  Bpp/Numeric/Hmm/LogsumHmmLikelihood.h
  Bpp/Numeric/Hmm/LowMemoryRescaledHmmLikelihood.h
  Bpp/Numeric/Hmm/RescaledHmmLikelihood.h
  Bpp/Numeric/Matrix/EigenValue.h
  Bpp/Numeric/Matrix/LUDecomposition.h
  Bpp/Numeric/Matrix/Matrix.h
  Bpp/Numeric/Matrix/MatrixTools.h
  Bpp/Numeric/Prob/AbstractDiscreteDistribution.h
  Bpp/Numeric/Prob/BetaDiscreteDistribution.h
  Bpp/Numeric/Prob/ConstantDistribution.h
  Bpp/Numeric/Prob/DirichletDiscreteDistribution.h
  Bpp/Numeric/Prob/DiscreteDistribution.h
  Bpp/Numeric/Prob/ExponentialDiscreteDistribution.h
  Bpp/Numeric/Prob/GammaDiscreteDistribution.h
  Bpp/Numeric/Prob/GaussianDiscreteDistribution.h
  Bpp/Numeric/Prob/InvariantMixedDiscreteDistribution.h
  Bpp/Numeric/Prob/MixtureOfDiscreteDistributions.h
  Bpp/Numeric/Prob/MultipleDiscreteDistribution.h
  Bpp/Numeric/Prob/SimpleDiscreteDistribution.h
  Bpp/Numeric/Prob/TruncatedExponentialDiscreteDistribution.h
  Bpp/Numeric/Prob/UniformDiscreteDistribution.h
  Bpp/Numeric/Stat/StatTest.h 
)

# Build the static lib
add_library(bppcore-static STATIC ${CPP_FILES})
set_target_properties(bppcore-static
  PROPERTIES OUTPUT_NAME bpp-core
  CLEAN_DIRECT_OUTPUT 1
  )

# Build the shared lib
add_library(bppcore-shared SHARED ${CPP_FILES})
set_target_properties(bppcore-shared
  PROPERTIES OUTPUT_NAME bpp-core
  CLEAN_DIRECT_OUTPUT 1
  VERSION ${BPPCORE_VERSION}
  SOVERSION ${BPPCORE_VERSION_MAJOR}
  )

# Install libs
install(TARGETS bppcore-static bppcore-shared DESTINATION lib)

# Install headers
install(DIRECTORY Bpp/ DESTINATION include/Bpp FILES_MATCHING PATTERN ".svn" EXCLUDE PATTERN "*.h" PATTERN "*.all")

