<?php

/**
 * @file
 * Apsis API call wrappers
 *
 * @author Hasse Ramlev Hansen <hasse@reload.dk>
 */

@require_once(drupal_get_path('module', 'emf_apsis') .'/APSIS.class.php');

/**
 * Implements hook_api_subscribe()
 */
function emf_apsis_api_subscribe($email, $fields, $lid) {
  $anp    = new AnpSoapclient(variable_get('emf_apsis_api_username', ''), variable_get('emf_apsis_api_password', ''));
  $return = $anp->Call('InsertSubscriberSid', array(
    'strEmail'         => $email,
    'strName'          => '',
    'strFormat'        => '',
    'strMailingListID' => $lid,
  ));
  // TODO: Add some error handling
  if ($return['InsertSubscriberResult'] < -10000) {
    //For now we assume subscribing worked as expected so return success
    return TRUE;
  }
  // Result codes between -1 and -7999 is error codes.
  return FALSE;
}

/**
 * Implements hook_api_unsubscribe()
 */
function emf_apsis_api_unsubscribe($email, $lid) {
  $anp    = new AnpSoapclient(variable_get('emf_apsis_api_username', ''), variable_get('emf_apsis_api_password', ''));
  $return = $anp->Call('DeleteSubscriber', array(
    'strEmail'         => $email,
    'strMailingListID' => $lid,
  ));
  //TODO: Add some error handling
  
  //For now we assume unsubscribing worked as expected so return success
  return TRUE;
}

/**
 * Implements hook_api_get_subscribers_unsubscribed()
 */
function emf_apsis_api_get_subscribers_unsubscribed($date = 0, $lid = NULL) {
  return array();
}

/**
 * Implements hook_api_get_subscribers_subscribed()
 */

function emf_apsis_api_get_subscribers_subscribed($date = 0, $lid = NULL) {
  $subscribers     = array();
  /* Removed as this causes out of memory errors during NuSOAP XML parsing of the response 
   * if the list contains many subscribers.
   * TODO: Consider using PHP internal SOAP library instead
  $anp             = new AnpSoapclient(variable_get('emf_apsis_api_username', ''), variable_get('emf_apsis_api_password', ''));
  $all_subscribers = $anp->call('GetSubscribers', array(
		'strMailingListId' => $lid,
  ));

  if (is_array($all_subscribers) && isset($all_subscribers['GetSubscribersResult']['diffgram']['NewDataSet']['Subscriber'])) {
	  foreach ($all_subscribers['GetSubscribersResult']['diffgram']['NewDataSet']['Subscriber'] as $subscriber) {
	    $subscribers[] = $subscriber['Email'];
	  }
  } else {
  	//TODO: Add some error handling
  }*/
  
  return $subscribers;
}

/**
 * Implements hook_api_get_lists()
 */
function emf_apsis_api_get_lists() {
  $lists        = array();
  $anp          = new AnpSoapclient(variable_get('emf_apsis_api_username', ''), variable_get('emf_apsis_api_password', ''));
  $mailinglists = $anp->call('GetMailingLists');

  if (is_array($mailinglists) && isset($mailinglists['GetMailingListsResult']['diffgram']['NewDataSet']['MailingList'])) {
	  foreach ($mailinglists['GetMailingListsResult']['diffgram']['NewDataSet']['MailingList'] as $list) {
	    $lists[$list['MailingListID']] = (object) array(
	      'lid'      => $list['MailingListID'],
	      'name_api' => $list['Name'],
	    );
	  }
  } else {
  	//TODO: Add some error handling
  }

  return $lists;
}

/**
 * Implements hook_api_get_custom_fields()
 */
function emf_apsis_api_get_custom_fields($lid) {
	return array();
}

/**
 * Implements hook_api_get_system_time()
 */
function emf_apsis_api_get_system_time() {
  return time() - 3600;
}

/**
 * Implements hook_api_unix_to_service_time()
 */
function emf_apsis_api_unix_to_service_time($date) {
  return $date;
}