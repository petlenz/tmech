cmake_minimum_required(VERSION 3.5)

project(tmech CXX)

set(TMECH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TMECH_TENSOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/tmech/tensor)
set(TMECH_SYMDIFF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/tmech/symdiff)

 if (MSVC)
    if(${MSVC_TOOLSET_VERSION} LESS 142)
        add_compile_definitions(TMECH_IS_DETECTED)
    endif()
endif()
#add_compile_definitions(TMECH_IS_DETECTED)

# Versionning
# ===========

file(STRINGS "${TMECH_INCLUDE_DIR}/tmech/tmech_config.h" tmech_version_defines
     REGEX "#define TMECH_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${tmech_version_defines})
    if(ver MATCHES "#define TMECH_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(TMECH_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(${PROJECT_NAME}_VERSION
    ${TMECH_VERSION_MAJOR}.${TMECH_VERSION_MINOR}.${TMECH_VERSION_PATCH})
message(STATUS "Building tmech v${${PROJECT_NAME}_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TMECH_TENSOR_HEADER
    ${TMECH_INCLUDE_DIR}/tmech/tensor/abs_tensor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/abs_tensor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/adaptor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/adaptor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/adjoint_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/adjoint_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/basis_change_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/basis_change_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/cofactor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/cofactor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/cross_product_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/cross_product_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/deviatoric_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/deviatoric_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/dot_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/dot_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenbasis_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenbasis_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenvalues_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenvalues_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenvectors_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/eigen_decomposition_eigenvectors_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/evaluation_temp_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/evaluation_temp_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/exp_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/exp_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/expression_operators.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/gemm_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/gemm_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/general_lu_decomposition_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/general_lu_decomposition_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/general_newton_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/general_newton_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/identity_tensor_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/identity_tensor_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/inner_product_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/inner_product_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/inverse_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/inverse_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/isotropic_tensor_functions_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/isotropic_tensor_functions_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/levi_civita_symbol_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/levi_civita_symbol_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/negative_tensor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/negative_tensor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/numerical_differentiation_central_sym_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/numerical_differentiation_central_sym_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/numerical_differentiation_central_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/numerical_differentiation_central_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/outer_product_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/outer_product_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_R_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_R_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_U_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_U_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_V_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_V_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dR_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dR_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dU_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dU_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dV_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/polar_decomposition_dV_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_dneg_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_dneg_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_dpos_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_dpos_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_neg_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_neg_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_pos_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/positive_negative_decomposition_pos_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/pow_tensor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/pow_tensor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/randn_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/randn_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/randu_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/randu_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/scalar_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/scalar_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/sign_tensor_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/sign_tensor_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/skew_symmetric_part_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/skew_symmetric_part_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/symmetric_part_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/symmetric_part_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/volumetric_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/volumetric_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/zero_tensor_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/zero_tensor_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_expression_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_expression_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/traits.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/utility.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_operators.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_functions_forward.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_functions_implementation.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/math_details.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/meta_inner_product_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_base_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_base_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/compare_almost_equal.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/compare_lambda.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/convert_tensor_to_voigt.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/pow_tensor_derivative_wrapper_bones.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/pow_tensor_derivative_wrapper_meat.h
    ${TMECH_INCLUDE_DIR}/tmech/tmech_forward_definitions.h
    ${TMECH_INCLUDE_DIR}/tmech/tensor/tensor_operators_forward.h
    )

set(TMECH_SYMDIFF_HEADER
    ${TMECH_SYMDIFF_INCLUDE_DIR}/variable_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/variable_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/variable_base_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/variable_base_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/binary_expression_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/binary_expression_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/symdiff_utility.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/symdiff_operators.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/symdiff_functions_forward.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/symdiff_functions_implementation.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/derivative_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/derivative_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/squeezer.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/scalar_one_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/scalar_one_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/scalar_zero_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/scalar_zero_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_one_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_one_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_outer_product_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_outer_product_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_zero_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_zero_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/real_constant_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/real_constant_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/forward_definitions.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_inner_product_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_inner_product_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_basis_change_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_basis_change_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/constant_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/constant_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/symdiff_type_traits.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/std_functions_implementation.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/std_functions_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/function_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/function_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/negative_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_inv_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_inv_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tmech_function_wrapper.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/negative_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tmech_functions_forward.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tmech_functions_implementation.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_function_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_function_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_isotropic_function_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_isotropic_function_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_isotropic_function_wrapper_derivative_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_isotropic_function_wrapper_derivative_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_as_sym_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_as_sym_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_pow_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_pow_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_pow_derivative_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_pow_derivative_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/jacobi_matrix_wrapper.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/newton_solver_multi_variable_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/newton_solver_single_variable_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/vector_of_functions.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/newton_raphson_solver_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/implicit_function_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/implicit_nonlinear_function_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_dev_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_dev_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_vol_wrapper_meat.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_vol_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_fourth_order_identity_wrapper_bones.h
    ${TMECH_SYMDIFF_INCLUDE_DIR}/tensor_fourth_order_identity_wrapper_meat.h
    )

# add the library that runs
if(${CMAKE_VERSION} VERSION_LESS "3.19.0")
add_library(${PROJECT_NAME} INTERFACE)
else()
add_library(${PROJECT_NAME} INTERFACE
    ${TMECH_INCLUDE_DIR}/tmech/tmech.h
    ${TMECH_INCLUDE_DIR}/tmech/tmech_config.h
    ${TMECH_TENSOR_HEADER}
    ${TMECH_SYMDIFF_HEADER}
    )
endif()
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GNUInstallDirs)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_TOPLEVEL_PROJECT FALSE)
endif()


OPTION(TMECH_INSTALL_LIBRARY
       "Enable installing of tmech library into default locations"
       ${IS_TOPLEVEL_PROJECT})
OPTION(BUILD_EXAMPLES "tmech examples" ON)
OPTION(BUILD_TESTS "tmech test suite" ON)
OPTION(BUILD_BENCHMARK "tmech benchmark" OFF)
OPTION(DOWNLOAD_GTEST "build gtest from downloaded sources" OFF)
OPTION(DOWNLOAD_GBENCHMARK "download google benchmark and build from source" OFF)



# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUIcd build && ctestnstallDirs)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)


if(TMECH_INSTALL_LIBRARY)
  # locations are provided by GNUInstallDirs
  install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION {${PROJECT_NAME}_VERSION}
    COMPATIBILITY SameMajorVersion)

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(
    EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/tmech
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()


set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}")

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

#if(BUILD_BENCHMARK)
#    add_subdirectory(benchmark)
#endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include(CPack)
