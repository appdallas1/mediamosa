<?php
/**
 * @file
 * The MediaMosa App class.
 */

class mediamosa_app {
  // ---------------------------------------------------------------- Functions.
  /**
   * Get the app using the app_id.
   *
   * @param integer $app_id
   *   The client application ID.
   *
   * @return array
   *   The application array.
   */
  public static function get_by_appid($app_id) {
    static $apps = array();

    // Any cache?
    if (!empty($apps[$app_id])) {
      return $apps[$app_id];
    }

    // Store in cache.
    $apps[$app_id] = mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->fields('app')
      ->condition(mediamosa_app_db::APP_ID, $app_id)
      ->execute()
      ->fetchAssoc();

    return $apps[$app_id];
  }

  /**
   * Get the app using the app_id.
   *
   * @param array $app_ids
   *   The application IDs.
   * @param array $fields
   *   For specific fields.
   *
   * @return DatabaseStatementInterface
   *   The result.
   */
  public static function get_by_appids(array $app_ids, array $fields = array()) {
    // Build query.
    $query = mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app');
    if (!empty($fields)) {
      foreach ($fields as $key => $field) {
        $query->addField('app', $field, (is_numeric($key) ? $field : $key));
      }
    }
    else {
      $query->fields('app');
    }

    return $query
      ->condition(mediamosa_app_db::APP_ID, $app_ids, 'IN')
      ->execute();
  }

  /**
   * Get the app using the node ID.
   *
   * @param integer $nid
   *   The node ID.
   * @param array $fields
   *   For specific columns.
   *
   * @return array
   *   The application or FALSE.
   */
  public static function get_by_nid($nid, $fields = array()) {
    return mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->fields('app', $fields)
      ->condition(mediamosa_app_db::NID, $nid)
      ->execute()
      ->fetchAssoc();
  }

  /**
   * Get the app using the oauth consumer key.
   *
   * @param string $oauth_consumer_key
   * @return array
   */
  public static function get_by_oauth_consumer_key($oauth_consumer_key, $fields = array()) {
    return mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->fields('app', $fields)
      ->condition(mediamosa_app_db::OAUTH_CONSUMER_KEY, $oauth_consumer_key)
      ->execute()
      ->fetchAssoc();
  }

  /**
   * Get all apps.
   *
   * @return DatabaseStatementInterface
   */
  public static function get_all_apps(array $fields = array(), $order_by = NULL, $order_direction = 'ASC') {
    $query = mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app');

    if (count($fields)) {
      foreach ($fields as $key => $field) {
        $query->addField('app', $field, (is_numeric($key) ? $field : $key));
      }
    }
    else {
      $query->fields('app', array(mediamosa_app_db::APP_ID, mediamosa_app_db::APP_NAME));
    }

    // Order the results
    if (isset($order_by)) {
      $query->orderBy($order_by, $order_direction);
    }

    // Dont change with ->fetchAll(), some callers expect the object here.
    return $query->execute();
  }

  /**
   * Get the app.
   *
   * @param string $name
   * @return array
   */
  public static function get_by_name($name) {
    return mediamosa_db::db_query(
      "SELECT * FROM {#table_name} WHERE #name = :name",
      array(
        '#name' => mediamosa_app_db::APP_NAME,
        '#table_name' => mediamosa_app_db::TABLE_NAME,
        ':name' => $name,
      )
    )->fetchAssoc();
  }

  /**
   * Update an application. Used for internal code.
   */
  public static function update($app_id, array $fields = array()) {

   // Update.
    return mediamosa_db::db_update(mediamosa_app_db::TABLE_NAME)
      ->fields($fields)
      ->condition(mediamosa_app_db::APP_ID, $app_id)
      ->execute();
  }

  /**
   * Returns all owners as app_id => owner.
   *
   * Used to return the owner field in the mediamosa_app table. Now returns all
   * Drupal users that have an permission that allows access to a client app.
   *
   * @return array
   *   The application owners.
   */
  public static function collect_app_owners() {
    global $user;

    // Add the roles of the apps.
    $owners = array();

    $query = mediamosa_db::db_select('users', 'u');
    $query->join('node', 'n', 'n.uid = u.uid');
    $query->join(mediamosa_app_db::TABLE_NAME, 'app', 'app.nid = n.nid');
    $query->fields('u', array('name'));
    $query->fields('app', array(mediamosa_app_db::APP_ID));
    $query->condition('n.type', mediamosa_node::MEDIAMOSA_NODE_TYPE_APP);
    $app_owners = $query->execute()->fetchAll();

    // Is admin?
    $is_admin = user_access(mediamosa_permission::ADMIN_MEDIAMOSA);

    foreach ($app_owners as $app_owner) {
      if ($is_admin || $user->name == $app_owner['name']) {
        $owners[$app_owner['name']][] = $app_owner['app_id'];
      }
    }

    return $owners;
  }

  /**
   * Collect the client apps as app_id => title where the current user has
   * access to.
   */
  public static function collect_allowed_apps() {
    $apps = array();
    foreach (mediamosa_app::get_all_apps(array(mediamosa_app_db::APP_ID, mediamosa_app_db::APP_NAME), mediamosa_app_db::APP_NAME, 'ASC') as $app) {
      if (!user_access(mediamosa_permission::ADMIN_MEDIAMOSA) && !_mediamosa_user_access(mediamosa_permission::ADMIN_MEDIAMOSA . '|app|' . $app[mediamosa_app_db::APP_ID])) {
        continue;
      }
      $apps[$app[mediamosa_app_db::APP_ID]] = "({$app[mediamosa_app_db::APP_ID]}) " . $app[mediamosa_app_db::APP_NAME];
    }
    return $apps;
  }

  /**
   * return an array of all apps that are allowed to master/slave this app.
   *
   * @param integer $app_id
   */
  public static function get_allowed_master_slave_apps($app_id) {
    $allowed_apps = mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->fields('app', array(mediamosa_app_db::ALLOW_MASTERSLAVE_APPS))
      ->condition(mediamosa_app_db::APP_ID, $app_id)
      ->execute()
      ->fetchField();

    if (empty($allowed_apps)) {
      return array();
    }

    // Return stored array.
    return unserialize($allowed_apps);
  }

  /**
   * Return the default value of the still, depending of the application id.
   *
   * @param integer $app_id
   *   The application ID.
   *
   * @return string|boolean
   *   Still default size or FALSE.
   */
  public static function get_still_default_size($app_id) {
    if (!$app_id) {
      return FALSE;
    }

    // Get the default size of still of application.
    $app = self::get_by_appid($app_id);

    if (!empty($app[mediamosa_app_db::STILL_DEFAULT_SIZE])) {
      return $app[mediamosa_app_db::STILL_DEFAULT_SIZE];
    }

    return FALSE;
  }

  /**
   * Return the still padding value, depending of the application id.
   *
   * @param integer $app_id
   */
  public static function get_still_padding_value($app_id) {
    if (!$app_id) {
      return mediamosa_app_db::STILL_PADDING_YES;
    }

    $app = self::get_by_appid($app_id);
    return (!empty($app[mediamosa_app_db::STILL_PADDING]) ? $app[mediamosa_app_db::STILL_PADDING] : mediamosa_app_db::STILL_PADDING_YES);
  }

  /**
   * Return the still maintain aspect ratio value, depending of the application
   * id.
   *
   * @param integer $app_id
   */
  public static function get_still_maintain_aspect_ratio_value($app_id) {
    if (!$app_id) {
      return mediamosa_app_db::STILL_MAINTAIN_ASPECT_RATIO_YES;
    }

    $app = self::get_by_appid($app_id);
    return (!empty($app[mediamosa_app_db::STILL_MAINTAIN_ASPECT_RATIO]) ? $app[mediamosa_app_db::STILL_MAINTAIN_ASPECT_RATIO] : mediamosa_app_db::STILL_MAINTAIN_ASPECT_RATIO_YES);
  }

  /**
   * Returns number of apps found in the database.
   */
  public static function get_number_of_apps() {
    return mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->countQuery()
      ->execute()
      ->fetchField();
  }

  /**
   * Get all apps.
   *
   * @return array
   */
  public static function get_all_apps_cached($options = array()) {
    $apps = &drupal_static(__CLASS__ . '::' . __FUNCTION__);

    $options += array(
      'order_by' => mediamosa_app_db::APP_NAME,
      'order_direction' => 'ASC',
    );

    $query = mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app');
    $query->fields('app', array(mediamosa_app_db::APP_ID, mediamosa_app_db::APP_NAME));

    // Order the results
    $query->orderBy($options['order_by'], $options['order_direction']);

    // Dont change with ->fetchAll(), some callers expect the object here.
    $apps[$options['order_by']][$options['order_direction']] = $query->execute()->fetchAll();

    // Return the apps.
    return $apps[$options['order_by']][$options['order_direction']];
  }

  /**
   * Test if application is enabled (active).
   *
   * @param string $name
   *   The name of the application.
   *
   * @return boolean
   *   Returns TRUE when app is active OR FALSE when not.
   */
  public static function is_app_active($name) {
    return mediamosa_db::db_select(mediamosa_app_db::TABLE_NAME, 'app')
      ->fields('app', array(mediamosa_app_db::ACTIVE))
      ->condition(mediamosa_app_db::APP_NAME, $name)
      ->execute()
      ->fetchField() == mediamosa_app_db::ACTIVE_TRUE;
  }

  /**
   * Return with the current authenticated client application ID from session.
   *
   * @return integer
   *   The client application ID or FALSE.
   */
  public static function get_session_app_id() {
    return empty($_SESSION['mediamosa_app']['app_id']) ? FALSE : (int) $_SESSION['mediamosa_app']['app_id'];
  }

  /**
   * Set the authenticated client application ID in session.
   *
   * @param integer $app_id
   *   The client application ID.
   */
  public static function set_session_app_id($app_id) {
    $_SESSION['mediamosa_app']['app_id'] = (int) $app_id;
  }

  /**
   * Logoff the client application from session.
   */
  public static function clear_session_app_id() {
    self::set_session_app_id(0);
  }
}
