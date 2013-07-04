<?php
/**
 * @file
 * API documentation MediaMosa.
 *
 * @todo: doc api hooks;
 *
 * hook_mediamosa_configuration_collect
 * hook_mediamosa_configuration_validate
 * hook_mediamosa_configuration_submit
 * hook_mediamosa_storage_info
 * hook_mediamosa_tool_can_analyse
 * hook_mediamosa_tool_can_generate_still
 * hook_mediamosa_tool_info
 * hook_mediamosa_asset_reindex
 * hook_mediamosa_asset_index_delete
 * hook_mediamosa_asset_queue
 * hook_mediamosa_register_rest_call
 * hook_mediamosa_register_rest_call_doc
 * hook_mediamosa_simpletest_clean_environment
 * hook_mediamosa_status_collect
 * hook_mediamosa_status_collect_realtime
 * hook_mediamosa_response_get
 * hook_mediamosa_app_authorized
 * hook_mediamosa_pass_call_drupal (needs rename).
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Return information about an metadata set.
 *
 * The metadata info hook describes the metadata set. The hook allows
 * registration of an metadata set to be used in MediaMosa.
 *
 * @return array
 *   An associative array, where main key is the main ID of the metadata set.
 *   - 'title'
 *     The title of the mediadata set.
 *   - 'description'
 *     The description of the metadata set.
 *   - 'context'
 *     A unigue ID, used as context ID: only alphanum only.
 *   - 'context_full'
 *     A unique long ID, alphum / underscore only.
 */
function hook_mediamosa_metadata_info() {
  return array(
    'dublin_core' => array(
      'title' => t('Dublin Core'),
      'description' => t('MediaMosa support for the metadata format Dublin Core. See !link for more information.', array('!link' => url('http://dublincore.org/'))),
      'context' => 'dc',
      'context_full' => 'dublin_core',
    ),
  );
}

/**
 * Return information about an search engine.
 *
 * Note:
 * Hook been renamed from 'mediamosa_search_engine' to
 * 'mediamosa_search_engine_info'.
 *
 * @return array
 *   An associative array, where main key is the main ID of the search engine.
 *   - 'title'
 *     The title of the search engine.
 *   - 'description'
 *     The description of the search engine.
 */
function hook_mediamosa_search_engine_info() {

  // Return the information about the default search engine.
  return array(
    'mediamosa_search' => array(
      'title' => t('MediaMosa default search'),
      'description' => t('The default search engine for searching in MediaMosa. This search engine is always available.'),
    ),
  );
}

/**
 * Process ACL changes to an object.
 *
 * When hook is called, the ACL right change has already been processed and the
 * ACL change was allowed.
 *
 * @param array $object
 *   The object that had the ACL change. This can be a asset,  mediafile or
 *   collection.
 * @param string $op
 *   The operation performed;
 *   - 'clear'
 *     All rights where cleared from the object.
 *   - 'add'
 *     Some rights where added.
 *   - 'replace'
 *     All rights where cleared and replaced.
 * @param string $acl_type
 *   The type of ACL, see mediamosa_acl::ACL_TYPE_*.
 * @param integer $app_id
 *   The application ID that called the ACL change.
 * @param string $user_id
 *   The owner of the object.
 * @param boolean $is_app_admin
 *   Is application admin.
 * @param array $acl_data
 *   An associative array containing the data for the 'set' operation. Is empty
 *   when called with operator 'clear'.
 *   - 'app_id_slaves'
 *     (array) Array of slaves set on app_id.
 *   - 'acl_user_ids' => $acl_user_ids,
 *     (array) Array of ACL user IDs.
 *   - 'acl_group_ids'
 *     (array) Array of ACL group user IDs.
 *   - 'acl_domains'
 *     (array) Array of ACL domains.
 *   - 'acl_realms'
 *     (array) Array of ACL realms.
 *   - 'rights'
 *     (array) Array of ACL rights (reserved). Is right default is
 *      mediamosa_acl::RIGHT_ACCESS.
 */
function hook_mediamosa_acl(array $object, $op, $acl_type, $app_id, $user_id, $is_app_admin, array $acl_data) {
}

/**
 * @} End of "addtogroup hooks".
 */
