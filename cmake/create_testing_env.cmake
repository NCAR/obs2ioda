set(VENV_DIR "${CMAKE_BINARY_DIR}/.obs2ioda" CACHE PATH "Path to the Python virtual environment")

if(WIN32)
    set(VENV_PYTHON "${VENV_DIR}/Scripts/python.exe")
    set(VENV_PIP "${VENV_DIR}/Scripts/pip.exe")
else()
    set(VENV_PYTHON "${VENV_DIR}/bin/python")
    set(VENV_PIP "${VENV_DIR}/bin/pip")
endif()

# Create virtual environment if it doesn't exist
if(NOT EXISTS "${VENV_PYTHON}")
    message(STATUS "Creating Python virtual environment at ${VENV_DIR}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E env python3 -m venv "${VENV_DIR}"
        RESULT_VARIABLE VENV_CREATE_RESULT
    )
    if(NOT VENV_CREATE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to create Python virtual environment.")
    endif()
endif()

# Upgrade pip
message(STATUS "Upgrading pip...")
execute_process(
    COMMAND "${VENV_PIP}" install --upgrade pip
    RESULT_VARIABLE PIP_UPGRADE_RESULT
)
if(NOT PIP_UPGRADE_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to upgrade pip in virtual environment.")
endif()

# Install required packages
set(PYTHON_PACKAGES netCDF4 pytest requests)
message(STATUS "Installing Python packages: ${PYTHON_PACKAGES}")
execute_process(
    COMMAND "${VENV_PIP}" install ${PYTHON_PACKAGES}
    RESULT_VARIABLE PIP_INSTALL_RESULT
)
if(NOT PIP_INSTALL_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to install required Python packages: ${PYTHON_PACKAGES}")
endif()
