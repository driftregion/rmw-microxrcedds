#include "rmw_micrortps.h"

#include "identifier.h"
#include "rmw_node.h"

#include <micrortps/client/client.h>

#include "rmw/allocators.h"
#include "rmw/error_handling.h"
#include "rosidl_typesupport_micrortps_c/identifier.h"

const char* rmw_get_implementation_identifier()
{
    EPROS_PRINT_TRACE()
    return eprosima_micrortps_identifier;
}

rmw_ret_t rmw_init()
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_node_t* rmw_create_node(const char* name, const char* namespace_, size_t domain_id,
                            const rmw_node_security_options_t* security_options)
{
    EPROS_PRINT_TRACE()
    if (!name)
    {
        RMW_SET_ERROR_MSG("name is null");
        return NULL;
    }
    if (!security_options)
    {
        RMW_SET_ERROR_MSG("security_options is null");
        return NULL;
    }
    return create_node(name, namespace_);
}

rmw_ret_t rmw_destroy_node(rmw_node_t* node)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

const rmw_guard_condition_t* rmw_node_get_graph_guard_condition(const rmw_node_t* node)
{
    (void) node;
    EPROS_PRINT_TRACE()
    rmw_guard_condition_t * ret = (rmw_guard_condition_t *)rmw_allocate(sizeof(rmw_guard_condition_t));
    ret->data = NULL;
    ret->implementation_identifier = eprosima_micrortps_identifier;
    return ret;   
}

rmw_publisher_t* rmw_create_publisher(const rmw_node_t* node, const rosidl_message_type_support_t* type_support,
                                      const char* topic_name, const rmw_qos_profile_t* qos_policies)
{
    EPROS_PRINT_TRACE()
        (void)qos_policies;
    rmw_publisher_t* ret           = (rmw_publisher_t*)rmw_allocate(sizeof(rmw_publisher_t));
    ret->data                      = NULL;
    ret->implementation_identifier = eprosima_micrortps_identifier;
    ret->topic_name                = topic_name;

    const rosidl_message_type_support_t * meessage_type_support = get_message_typesupport_handle(
    type_support, rosidl_typesupport_micrortps_c__identifier);
    if (!type_support) {
        RMW_SET_ERROR_MSG("type support not from this implementation");
        return NULL;
    }
    // rosidl_typesupport_introspection_c__MessageMembers* members = (rosidl_typesupport_introspection_c__MessageMembers*)meessage_type_support->data;
    // if (!members) {
    //     RMW_SET_ERROR_MSG("members handle is null");
    //     return NULL;
    // }

    MicroNode* micro_node = (MicroNode*)node->data;
    micro_node->publisher_id = mr_object_id(0x01, MR_PUBLISHER_ID);
    const char* publisher_xml = "<publisher name=\"MyPublisher\">";
    uint16_t publisher_req = mr_write_configure_publisher_xml(&micro_node->session, reliable_output, micro_node->publisher_id, micro_node->participant_id, publisher_xml, MR_REPLACE);
    
    mrObjectId topic_id = mr_object_id(0x01, MR_TOPIC_ID);
    const char* topic_xml = "<dds><topic><name>HelloWorldTopic</name><dataType>HelloWorld</dataType></topic></dds>";
    uint16_t topic_req = mr_write_configure_topic_xml(&micro_node->session, reliable_output, topic_id, micro_node->participant_id, topic_xml, MR_REPLACE);


    micro_node->datawriter_id = mr_object_id(0x01, MR_DATAWRITER_ID);
    const char* datawriter_xml = "<profiles><publisher profile_name=\"default_xrce_publisher_profile\"><topic><kind>NO_KEY</kind><name>HelloWorldTopic</name><dataType>HelloWorld</dataType><historyQos><kind>KEEP_LAST</kind><depth>5</depth></historyQos><durability><kind>TRANSIENT_LOCAL</kind></durability></topic></publisher></profiles>";
    uint16_t datawriter_req = mr_write_configure_datawriter_xml(&micro_node->session, reliable_output, micro_node->datawriter_id, micro_node->publisher_id, datawriter_xml, MR_REPLACE);
    uint8_t status[3];
    uint16_t requests[] = {publisher_req, datawriter_req, topic_req};
    if(!mr_run_session_until_status(&micro_node->session, 1000, requests, status, 3))
    {
        rmw_free(ret);
        ret = NULL;
    }
    return ret;
}

rmw_ret_t rmw_destroy_publisher(rmw_node_t* node, rmw_publisher_t* publisher)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_publish(const rmw_publisher_t* publisher, const void* ros_message)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_publish_serialized_message(const rmw_publisher_t* publisher,
                                         const rmw_serialized_message_t* serialized_message)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_serialize(const void* ros_message, const rosidl_message_type_support_t* type_support,
                        rmw_serialized_message_t* serialized_message)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_deserialize(const rmw_serialized_message_t* serialized_message,
                          const rosidl_message_type_support_t* type_support, void* ros_message)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_subscription_t* rmw_create_subscription(const rmw_node_t* node, const rosidl_message_type_support_t* type_support,
                                            const char* topic_name, const rmw_qos_profile_t* qos_policies,
                                            bool ignore_local_publications)
{
    EPROS_PRINT_TRACE()
    return NULL;
}

rmw_ret_t rmw_destroy_subscription(rmw_node_t* node, rmw_subscription_t* subscription)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take(const rmw_subscription_t* subscription, void* ros_message, bool* taken)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take_with_info(const rmw_subscription_t* subscription, void* ros_message, bool* taken,
                             rmw_message_info_t* message_info)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take_serialized_message(const rmw_subscription_t* subscription,
                                      rmw_serialized_message_t* serialized_message, bool* taken)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take_serialized_message_with_info(const rmw_subscription_t* subscription,
                                                rmw_serialized_message_t* serialized_message, bool* taken,
                                                rmw_message_info_t* message_info)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_client_t* rmw_create_client(const rmw_node_t* node, const rosidl_service_type_support_t* type_support,
                                const char* service_name, const rmw_qos_profile_t* qos_policies)
{
    EPROS_PRINT_TRACE()
    return NULL;
}

rmw_ret_t rmw_destroy_client(rmw_node_t* node, rmw_client_t* client)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_send_request(const rmw_client_t* client, const void* ros_request, int64_t* sequence_id)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take_response(const rmw_client_t* client, rmw_request_id_t* request_header, void* ros_response,
                            bool* taken)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_service_t* rmw_create_service(const rmw_node_t* node, const rosidl_service_type_support_t* type_support,
                                  const char* service_name, const rmw_qos_profile_t* qos_policies)
{
    EPROS_PRINT_TRACE()
    return NULL;
}

rmw_ret_t rmw_destroy_service(rmw_node_t* node, rmw_service_t* service)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_take_request(const rmw_service_t* service, rmw_request_id_t* request_header, void* ros_request,
                           bool* taken)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_send_response(const rmw_service_t* service, rmw_request_id_t* request_header, void* ros_response)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_guard_condition_t* rmw_create_guard_condition(void)
{
    EPROS_PRINT_TRACE()
    return NULL;
}

rmw_ret_t rmw_destroy_guard_condition(rmw_guard_condition_t* guard_condition)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_trigger_guard_condition(const rmw_guard_condition_t* guard_condition)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_wait_set_t* rmw_create_wait_set(size_t max_conditions)
{
    EPROS_PRINT_TRACE()
    (void)max_conditions;
    rmw_wait_set_t* wait_set            = (rmw_wait_set_t*)rmw_allocate(sizeof(rmw_wait_set_t));
    wait_set->data                      = NULL;
    wait_set->guard_conditions          = NULL;
    wait_set->implementation_identifier = rmw_get_implementation_identifier();
    return wait_set;
}

rmw_ret_t rmw_destroy_wait_set(rmw_wait_set_t* wait_set)
{
    EPROS_PRINT_TRACE()
    rmw_free(wait_set);
    return RMW_RET_OK;
}

rmw_ret_t rmw_wait(rmw_subscriptions_t* subscriptions, rmw_guard_conditions_t* guard_conditions,
                   rmw_services_t* services, rmw_clients_t* clients, rmw_wait_set_t* wait_set,
                   const rmw_time_t* wait_timeout)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_get_node_names(const rmw_node_t* node, rcutils_string_array_t* node_names)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_count_publishers(const rmw_node_t* node, const char* topic_name, size_t* count)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_count_subscribers(const rmw_node_t* node, const char* topic_name, size_t* count)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_get_gid_for_publisher(const rmw_publisher_t* publisher, rmw_gid_t* gid)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_compare_gids_equal(const rmw_gid_t* gid1, const rmw_gid_t* gid2, bool* result)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_service_server_is_available(const rmw_node_t* node, const rmw_client_t* client, bool* is_available)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_set_log_6severity(rmw_log_severity_t severity)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_get_topic_names_and_types(const rmw_node_t* node, rcutils_allocator_t* allocator, bool no_demangle,
                                        rmw_names_and_types_t* topic_names_and_types)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}

rmw_ret_t rmw_get_service_names_and_types(const rmw_node_t* node, rcutils_allocator_t* allocator,
                                          rmw_names_and_types_t* service_names_and_types)
{
    EPROS_PRINT_TRACE()
    return RMW_RET_OK;
}