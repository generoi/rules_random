<?php

/**
 * Implements hook_views_data().
 */
function rules_random_views_data() {
  // Define the base group of this table.
  $data['rules_random']['table']['group'] = t('Random winner');

  // Advertise this table as a possible base table
  $data['rules_random']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Random winner'),
    'help' => t('Random winner'),
    'weight' => -10,
  );

  $data['rules_random']['id'] = array(
    'title' => t('ID'),
    'help' => t('Unique identifier'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['rules_random']['cid'] = array(
    'title' => t('Draw dentifier'),
    'help' => t('Unique limitation identifier'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['rules_random']['value'] = array(
    'title' => t('Value'),
    'help' => t('Custom value identifier the winner'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['rules_random']['created'] = array(
    'title' => t('Created'),
    'help' => t('The time this winner was selected'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  return $data;
}
