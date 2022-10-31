cmake_minimum_required(VERSION 3.19)
PROJECT(mosquitto_example C CXX)

set(CMAKE_CXX_STANDARD 17)

add_link_options(-export-dynamic)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(spdlog REQUIRED)

# add_library(dlopenRos SHARED src/Turtlesim_auto.cpp)
# ament_target_dependencies(dlopenRos rclcpp geometry_msgs)
include(cmake/common.cmake)

# Find mosquitto
set(MOSQUITTO_ROOT "scripts/mosquitto/usr/local")
include(cmake/findmosquitto.cmake)
find_mosquitto()

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${MOSQUITTO_INCLUDE_DIR})

get_filename_component(msg_proto "/home/jessewl/work/C-Programs/gRPC-mqtt/proto/Message.proto" ABSOLUTE)
get_filename_component(msg_proto_path "${msg_proto}" PATH)

# PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS "${msg_proto}")
set(msg_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/Message.pb.cc")
set(msg_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Message.pb.h")
set(msg_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/Message.grpc.pb.cc")
set(msg_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Message.grpc.pb.h")
add_custom_command(
        OUTPUT "${msg_proto_srcs}" "${msg_proto_hdrs}" "${msg_grpc_srcs}" "${msg_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${msg_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${msg_proto}")

add_library(dlopenRos SHARED src/Turtlesim_auto.cpp)
ament_target_dependencies(dlopenRos rclcpp geometry_msgs )
target_link_libraries(dlopenRos ${MOSQUITTO_LIBRARIES})

ADD_EXECUTABLE(sub_callback src/sub_callback.cpp ${msg_proto_srcs} ${msg_proto_hdrs} ${msg_grpc_srcs} ${msg_grpc_hdrs})
ament_target_dependencies(sub_callback rclcpp geometry_msgs)
target_link_libraries(sub_callback dlopenRos spdlog::spdlog ${_PROTOBUF_LIBPROTOBUF} ${MOSQUITTO_LIBRARIES} ${_REFLECTION} ${_GRPC_GRPCPP})

ADD_EXECUTABLE(pub_simple src/pub_simple.cpp ${msg_proto_srcs} ${msg_proto_hdrs} ${msg_grpc_srcs} ${msg_grpc_hdrs})
target_link_libraries(pub_simple dlopenRos ${_PROTOBUF_LIBPROTOBUF} ${MOSQUITTO_LIBRARIES} ${_REFLECTION} ${_GRPC_GRPCPP})
