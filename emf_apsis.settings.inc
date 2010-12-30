<?php

/**
 * @file
 * Settings functions and callbacks.
 *
 * @author Hasse R. Hansen
 */

/**
 * Generate a form which handles Apsis login information
 */
function emf_apsis_settings() {
  $form = array();

  $form['emf_apsis_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API Username'),
    '#description' => t('Your Apsis API Username.'),
    '#default_value' => variable_get('emf_apsis_api_username', ''),
    '#required' => TRUE,
  );
  $form['emf_apsis_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('API Password'),
    '#description' => t('Your Apsis API Password.'),
    '#default_value' => variable_get('emf_apsis_api_password', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
