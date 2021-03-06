// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rmw_uros/options.h"

#include "types.h"

#include <rmw_microxrcedds_c/config.h>
#include <rmw/allocators.h>
#include <rmw/ret_types.h>
#include <rmw/error_handling.h>

rmw_ret_t rmw_uros_init_options(
  int argc, const char * const argv[],
  rmw_init_options_t * rmw_options)
{
  if (NULL == rmw_options) {
    RMW_SET_ERROR_MSG("Uninitialised rmw_init_options.");
    return RMW_RET_INVALID_ARGUMENT;
  }
  rmw_ret_t ret = RMW_RET_OK;
  // TODO(pablogs9): Is the impl allocated at this point?
  // rmw_options->impl = rmw_options->allocator.allocate(
  // sizeof(rmw_init_options_impl_t),
  // rmw_options->allocator.state);
#if defined(MICRO_XRCEDDS_SERIAL) || defined(MICRO_XRCEDDS_CUSTOM_SERIAL)
  if (argc >= 2) {
    strcpy(rmw_options->impl->connection_params.serial_device, argv[1]);
  } else {
    RMW_SET_ERROR_MSG(
      "Wrong number of arguments in rmw options. Needs one argument with the serial device.");
    ret = RMW_RET_INVALID_ARGUMENT;
  }

#elif defined(MICRO_XRCEDDS_UDP)
  if (argc >= 3) {
    strcpy(rmw_options->impl->connection_params.agent_address, argv[1]);
    strcpy(rmw_options->impl->connection_params.agent_port, argv[2]);
  } else {
    RMW_SET_ERROR_MSG("Wrong number of arguments in rmw options. Needs an Agent IP and port.");
    ret = RMW_RET_INVALID_ARGUMENT;
  }
#else
  (void) argc;
  (void) argv;
#endif
  return ret;
}

rmw_ret_t rmw_uros_options_set_serial_device(const char * dev, rmw_init_options_t * rmw_options)
{
#if defined(MICRO_XRCEDDS_SERIAL) || defined(MICRO_XRCEDDS_CUSTOM_SERIAL)
  if (NULL == rmw_options) {
    RMW_SET_ERROR_MSG("Uninitialised rmw_init_options.");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (dev != NULL && strlen(dev) <= MAX_SERIAL_DEVICE) {
    strcpy(rmw_options->impl->connection_params.serial_device, dev);
  } else {
    RMW_SET_ERROR_MSG("serial port configuration error");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return RMW_RET_OK;
#else
  (void) dev;
  (void) rmw_options;

  RMW_SET_ERROR_MSG("MICRO_XRCEDDS_SERIAL not set.");
  return RMW_RET_INVALID_ARGUMENT;
#endif
}

rmw_ret_t rmw_uros_options_set_udp_address(
  const char * ip, const char * port,
  rmw_init_options_t * rmw_options)
{
#ifdef MICRO_XRCEDDS_UDP
  if (NULL == rmw_options) {
    RMW_SET_ERROR_MSG("Uninitialised rmw_init_options.");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (ip != NULL && strlen(ip) <= MAX_IP_LEN) {
    strcpy(rmw_options->impl->connection_params.agent_address, ip);
  } else {
    RMW_SET_ERROR_MSG("default ip configuration error");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (port != NULL && strlen(port) <= MAX_PORT_LEN) {
    strcpy(rmw_options->impl->connection_params.agent_port, port);
  } else {
    RMW_SET_ERROR_MSG("default port configuration error");
    return RMW_RET_INVALID_ARGUMENT;
  }

  return RMW_RET_OK;
#else
  (void) ip;
  (void) port;
  (void) rmw_options;

  RMW_SET_ERROR_MSG("MICRO_XRCEDDS_UDP not set.");
  return RMW_RET_INVALID_ARGUMENT;
#endif
}

rmw_ret_t rmw_uros_options_set_client_key(uint32_t client_key, rmw_init_options_t * rmw_options)
{
  if (NULL == rmw_options) {
    RMW_SET_ERROR_MSG("Uninitialised rmw_init_options.");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rmw_options->impl->connection_params.client_key = client_key;

  return RMW_RET_OK;
}
