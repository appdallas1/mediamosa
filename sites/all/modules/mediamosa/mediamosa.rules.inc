<?php
/**
 * @file
 * MediaMosa Rules with events, conditions and actions.
 */

/**
 * Implements hook_rules_event_info().
 */
function mediamosa_rules_event_info() {
  $events = array();
  $defaults = array(
    'group' => t('MediaMosa'),
    'module' => 'mediamosa',
  );

  // Upload.
  $events['mediamosa_event_upload_finished'] = $defaults + array(
    'label' => t('An upload finished'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );
  $events['mediamosa_event_upload_failed'] = $defaults + array(
    'label' => t('An upload failed'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );

  // Analyse.
  $events['mediamosa_event_analyse_finished'] = $defaults + array(
    'label' => t('An analyse finished'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );

  // Transcode.
  $events['mediamosa_event_transcode_started'] = $defaults + array(
    'label' => t('A transcode started'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );
  $events['mediamosa_event_transcode_finished'] = $defaults + array(
    'label' => t('A transcode finished'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );
  $events['mediamosa_event_transcode_failed'] = $defaults + array(
    'label' => t('A transcode failed'),
    'variables' => _mediamosa_rules_variable_mediafile_id(),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function mediamosa_rules_condition_info() {
  $conditions = array();

  $conditions['mediamosa_condition_mediafile_filesize'] = array(
    'label' => t('Mediafile filesize > number of bytes'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_filesize(),
  );
  $conditions['mediamosa_condition_mediafile_mimetype'] = array(
    'label' => t('Mediafile mimetype = ...'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_mimetype(),
  );
  $conditions['mediamosa_condition_mediafile_transcode_profile'] = array(
    'label' => t('Mediafile transcode profile = ...'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_transcode_profile_id(),
  );
  $conditions['mediamosa_condition_mediafile_video_codec'] = array(
    'label' => t('Mediafile video codec = ...'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_video_codec(),
  );
  $conditions['mediamosa_condition_mediafile_bitrate'] = array(
    'label' => t('Mediafile bitrate > ...'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_bitrate(),
  );
  $conditions['mediamosa_condition_mediafile_width'] = array(
    'label' => t('Mediafile width > ...'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_width(),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function mediamosa_rules_action_info() {
  $actions = array(
    'mediamosa_action_analyse' => array(
      'label' => t('Start an analyse job for a mediafile'),
      'group' => t('MediaMosa'),
      'parameter' => _mediamosa_rules_variable_mediafile_id(),
    ),
  );
  $actions['mediamosa_action_transcode'] = array(
    'label' => t('Start an transcode job for a mediafile'),
    'group' => t('MediaMosa'),
    'parameter' =>
      _mediamosa_rules_variable_mediafile_id() +
      _mediamosa_rules_variable_transcode_profile_id(),
  );

  $actions['mediamosa_action_delete_mediafile'] = array(
    'label' => t('Delete a mediafile'),
    'group' => t('MediaMosa'),
    'parameter' => _mediamosa_rules_variable_mediafile_id(),
  );

  $actions['mediamosa_action_delete_original_mediafile'] = array(
    'label' => t('Delete original mediafile'),
    'group' => t('MediaMosa'),
    'description' => t('Delete the *original* source mediafile of the id given, if any.'),
    'parameter' => _mediamosa_rules_variable_mediafile_id(),
  );
  return $actions;
}

/**
 * Defines variable mediafile_id.
 */
function _mediamosa_rules_variable_mediafile_id() {
  return array(
    'mediafile_id' => array(
      'type' => 'text',
      'label' => t('Mediafile id'),
    ),
  );
}
/**
 * Defines variable filesize.
 */
function _mediamosa_rules_variable_filesize() {
  return array(
    'filesize' => array(
      'label' => t('Filesize'),
      'type' => 'integer',
    ),
  );
}

/**
 * Defines variable mimetype.
 */
function _mediamosa_rules_variable_mimetype() {
  return array(
    'mimetype' => array(
      'type' => 'text',
      'label' => t('Mime Type'),
    ),
  );
}

/**
 * Defines variable transcode_profile_id.
 */
function _mediamosa_rules_variable_transcode_profile_id() {
  return array(
    'transcode_profile_id' => array(
      'type' => 'integer',
      'label' => t('Transcode profile id'),
    ),
  );
}

/**
 * Defines variable video_codec.
 */
function _mediamosa_rules_variable_video_codec() {
  return array(
    'videocodec' => array(
      'type' => 'text',
      'label' => t('Video Codec'),
    ),
  );
}

/**
 * Defines variable bitrate.
 */
function _mediamosa_rules_variable_bitrate() {
  return array(
    'bitrate' => array(
      'type' => 'text',
      'label' => t('Video bitrate'),
    ),
  );
}


/**
 * Defines variable width.
 */
function _mediamosa_rules_variable_width() {
  return array(
    'width' => array(
      'type' => 'text',
      'label' => t('width'),
    ),
  );
}

/**
 * The action function for analyse.
 */
function mediamosa_action_analyse($mediafile_id) {
  // Start an analyse job.
  if ($mediafile_id != NULL) {
    $fields = array(
      mediamosa_asset_mediafile_db::ID,
      mediamosa_asset_mediafile_db::ASSET_ID,
      mediamosa_asset_mediafile_db::APP_ID,
      mediamosa_asset_mediafile_db::OWNER_ID,
    );
    $mediafile = mediamosa_asset_mediafile::get($mediafile_id, NULL, $fields);

    $job = mediamosa_job::create_analyse_job(
      $mediafile['owner_id'],
      $mediafile['asset_id'],
      $mediafile['mediafile_id'],
      $mediafile['app_id'],
      $priority);
    return TRUE;
  }
  return FALSE;
}

/**
 * The action function for transcode.
 */
function mediamosa_action_transcode($mediafile_id, $transcode_profile_id) {

  if ($mediafile_id == NULL) {
    return FALSE;
  }

  $fields = array(
    mediamosa_asset_mediafile_db::ID,
    mediamosa_asset_mediafile_db::ASSET_ID,
    mediamosa_asset_mediafile_db::APP_ID,
    mediamosa_asset_mediafile_db::OWNER_ID,
    mediamosa_asset_mediafile_db::GROUP_ID,
  );

  $mediafile = mediamosa_asset_mediafile::get($mediafile_id, NULL, $fields);
  if (!$mediafile) {
    return FALSE;
  }

  $job_id = mediamosa_job::create_job_transcode(
    $mediafile['app_id'],
    $mediafile['owner_id'],
    $mediafile['group_id'],
    TRUE,
    $mediafile['mediafile_id'],
    FALSE, '', '', '', '', (int) $transcode_profile_id, '',
    0);

  if ($job_id > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * The action function for delete a mediafile.
 */
function mediamosa_action_delete_mediafile($mediafile_id) {
  // Delete a mediafile.
  if ($mediafile_id != NULL) {
    mediamosa_asset_mediafile::delete($mediafile_id);
    return TRUE;
  }
  return FALSE;
}

/**
 * The action function to delete an original mediafile.
 */
function mediamosa_action_delete_original_mediafile($mediafile_id) {

  // First search the original of the mediafile.
  $mediafile_orig_id = mediamosa_db::db_select('mediamosa_asset_mediafile','mf')
    ->fields('mf',array('mediafile_id_source'))
    ->range(0,1)
    ->condition('mf.mediafile_id', $mediafile_id,'=')
    ->execute()
    ->fetchField();

  // Delete the original mediafile.
  if (($mediafile_orig_id != NULL) && ($mediafile_orig_id != $mediafile_id)) {
    mediamosa_asset_mediafile::delete($mediafile_orig_id);
    return TRUE;
  }
  return FALSE;
}

/**
 * Rules condition filesize of a mediafile.
 *
 * Returns TRUE if size of $mediafile_id is greater then $filesize.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $filesize
 *   The value to check.
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_filesize($mediafile_id, $filesize = 0) {

  $mf_size = mediamosa_asset_mediafile_metadata::get_mediafile_metadata_int($mediafile_id, mediamosa_asset_mediafile_metadata::FILESIZE);
  return ($mf_size > $filesize) ? TRUE : FALSE;
}

/**
 * Rules condition mime-type of a mediafile.
 *
 * Returns TRUE if mimetype of $mediafile_id equal to $mimetype.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $mime_type
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_mimetype($mediafile_id, $mimetype = '') {

  $mf_mimetype = mediamosa_asset_mediafile_metadata::get_mediafile_metadata_char($mediafile_id, mediamosa_asset_mediafile_metadata::MIME_TYPE);
  return ($mf_mimetype == $mimetype) ? TRUE : FALSE;
}

/**
 * Rules condition videocodec of a mediafile.
 *
 * Returns TRUE if videotype of $mediafile_id equal to $videocodec.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $videocodec
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_video_codec($mediafile_id, $videocodec = '') {

  $mf_videocodec = mediamosa_asset_mediafile_metadata::get_mediafile_metadata_char($mediafile_id, mediamosa_asset_mediafile_metadata::VIDEO_CODEC);
  return ($mf_videocodec == $videocodec) ? TRUE : FALSE;
}

/**
 * Rules condition width (part of size) of a mediafile
 *
 * Returns TRUE if width of $mediafile_id > $width.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $width
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_width($mediafile_id, $width = '') {

  $mf_width = mediamosa_asset_mediafile_metadata::get_mediafile_metadata_int($mediafile_id, mediamosa_asset_mediafile_metadata::WIDTH);
  return ($mf_width > $width) ? TRUE : FALSE;
}

/**
 * Rules condition bitrate of a mediafile.
 *
 * Returns TRUE if bitrate of $mediafile_id > $bitrate.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $bitrate
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_bitrate($mediafile_id, $bitrate = '') {

  $mf_bitrate = mediamosa_asset_mediafile_metadata::get_mediafile_metadata_int($mediafile_id, mediamosa_asset_mediafile_metadata::BITRATE);
  return ($mf_bitrate > $bitrate) ? TRUE : FALSE;
}

/**
 * Rules condition transcode profile of a mediafile.
 *
 * Returns TRUE if transcode_profile_id of $mediafile_id equal to $transcode_profile_id.
 *
 * @param string $mediafile_id
 *   Mediafile id to check against.
 * @param string $transcode_profile_id
 *
 * @return boolean
 *   returns the value of the condition.
 */
function mediamosa_condition_mediafile_transcode_profile($mediafile_id, $transcode_profile_id = '') {

  $mf_profile_id = mediamosa_db::db_select('mediamosa_asset_mediafile','mf')
    ->fields('mf', array(mediamosa_asset_mediafile_db::TRANSCODE_PROFILE_ID))
    ->range(0,1)
    ->condition('mf.mediafile_id', $mediafile_id,'=')
    ->execute()
    ->fetchField();

  return ($mf_profile_id == $transcode_profile_id) ? TRUE : FALSE;
}
