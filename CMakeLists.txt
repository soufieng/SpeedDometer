cmake_minimum_required(VERSION 3.22)

project(teamwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UART_COM OFF)

set(CLIENT_DIR client/desktop)
set(SERVER_DIR server/desktop)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets Multimedia SerialPort)

set(QT_LIBRARIES_CL Qt6::Core Qt6::Widgets Qt6::Multimedia)
set(QT_LIBRARIES_SV Qt6::Core Qt6::Widgets)

if(${UART_COM} MATCHES ON)

    add_compile_definitions(UART_COM)

    set(CLIENT_HEADERS shared/setting.h ${CLIENT_DIR}/include/window.h ${CLIENT_DIR}/include/canvas.h ${CLIENT_DIR}/include/comservice.h ${CLIENT_DIR}/include/uartcom.h)
    set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp ${CLIENT_DIR}/src/window.cpp ${CLIENT_DIR}/src/canvas.cpp ${CLIENT_DIR}/src/comservice.cpp ${CLIENT_DIR}/src/uartcom.cpp)
    set(QT_LIBRARIES_CL ${QT_LIBRARIES_CL} Qt6::SerialPort)

    set(SERVER_HEADERS shared/setting.h ${SERVER_DIR}/include/dialog.h ${SERVER_DIR}/include/comservices.h ${SERVER_DIR}/include/uartcoms.h)
    set(SERVER_SOURCES ${SERVER_DIR}/main.cpp ${SERVER_DIR}/src/dialog.cpp ${SERVER_DIR}/src/comservices.cpp ${SERVER_DIR}/src/uartcoms.cpp)
    set(QT_LIBRARIES_SV ${QT_LIBRARIES_SV} Qt6::SerialPort)

else()

    set(CLIENT_HEADERS shared/setting.h ${CLIENT_DIR}/include/window.h ${CLIENT_DIR}/include/canvas.h ${CLIENT_DIR}/include/comservice.h ${CLIENT_DIR}/include/tcpservice.h)
    set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp ${CLIENT_DIR}/src/window.cpp ${CLIENT_DIR}/src/canvas.cpp ${CLIENT_DIR}/src/comservice.cpp ${CLIENT_DIR}/src/tcpservice.cpp)

    set(SERVER_HEADERS shared/setting.h ${SERVER_DIR}/include/dialog.h ${SERVER_DIR}/include/comservices.h ${SERVER_DIR}/include/tcpservices.h)
    set(SERVER_SOURCES ${SERVER_DIR}/main.cpp ${SERVER_DIR}/src/dialog.cpp ${SERVER_DIR}/src/comservices.cpp ${SERVER_DIR}/src/tcpservices.cpp)

endif()

add_executable(client ${CLIENT_HEADERS} ${CLIENT_SOURCES})
target_link_libraries(client PUBLIC ${QT_LIBRARIES_CL} )
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include)

add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES})
target_link_libraries(server PUBLIC ${QT_LIBRARIES_SV} )
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include)

add_custom_target(upload_client cd ${CMAKE_SOURCE_DIR}/client/esp32 && pio run --target upload)
add_custom_target(upload_server cd ${CMAKE_SOURCE_DIR}/server/esp32 && pio run --target upload)
