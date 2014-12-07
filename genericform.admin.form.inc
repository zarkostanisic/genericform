<?php
/**
* @file
*
* Admin form
*
* This form provides administrator ability to create input form.
*
*/
function admin_form($form, $form_state, $no_js_use = FALSE) {
  $form = array();
  $form['#tree'] = TRUE;
  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['adminform'] = array(
    '#type' => 'vertical_tabs',
  );

  //PART of Class start
  $form['admin_class'] = array(
    '#type' => 'fieldset',
    '#title' => t('Class'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-admin_class')),
    '#group' => 'adminform',
  );

  $form['admin_class']['class_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name for class input.'),
    '#default_value' => variable_get('genericform_class_name'),
    '#required' => TRUE,
  );

  $form['admin_class']['class_fieldset'] = array(
    '#type' => 'textfield',
    '#title' => t('Fieldset name'),
    '#description' => t('Name for class fieldset.'),
    '#default_value' => variable_get('genericform_class_fieldset'),
    '#required' => TRUE,
  );

  $form['admin_class']['add_class'] = array(
    '#name' => 'add_class',
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('save_class'),
    '#limit_validation_errors' => array(
      array('admin_class', 'class_name'),
      array('admin_class', 'class_fieldset'),
    ),
  );
  //PART of Class end
  //PART of Item start
  $form['admin_item'] = array(
    '#type' => 'fieldset',
    '#title' => t('Item'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-admin_item')),
    '#group' => 'adminform',
  );

  $form['admin_item']['item_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name for item input.'),
    '#default_value' => variable_get('genericform_item_name'),
    '#required' => TRUE,
  );

  $form['admin_item']['item_fieldset'] = array(
    '#type' => 'textfield',
    '#title' => t('Fieldset name'),
    '#description' => t('Name for item fieldset.'),
    '#default_value' => variable_get('genericform_item_fieldset'),
    '#required' => TRUE,
  );

  $form['admin_item']['add_item'] = array(
    '#name' => 'add_item',
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('save_item'),
    '#limit_validation_errors' => array(
      array('admin_item', 'item_name'),
      array('admin_item', 'item_fieldset'),
    ),
  );
  //PART of Item end
  //PART of Attributes start
  $form['admin_attributes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attributes'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-admin_attributes')),
    '#group' => 'adminform',
  );

  $form['admin_attributes']['attributes_fieldset'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="attributes-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_attributes'])) {
      $form_state['num_attributes'] = 1;
  }

  for ($i = 0; $i < $form_state['num_attributes']; $i++) {
    $form['admin_attributes']['attributes_fieldset']['attributes'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('Field for attribute name.'),
      '#attributes' => array('multiple' => 'multiple'),
      '#required' => TRUE,
    );
  }

  $form['admin_attributes']['attributes_fieldset']['add_attribute'] = array(
    '#name' => 'add_attribute',
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('attributes_add_more_add_one'),
    '#limit_validation_errors' => array(
      array('admin_attributes', 'attributes_fieldset', 'attributes')
    ),
    '#ajax' => array(
      'callback' => 'attributes_add_more_callback',
      'wrapper' => 'attributes-fieldset-wrapper',
    ),
  );

  if($form_state['num_attributes'] > 1){
    $form['admin_attributes']['attributes_fieldset']['del_attribute'] = array(
      '#name' => 'del_attribute',
      '#type' => 'submit',
      '#value' => t('-'),
      '#submit' => array('attributes_add_more_del_one'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'attributes_add_more_callback',
        'wrapper' => 'attributes-fieldset-wrapper',
      ),
    );
  }

  $form['admin_attributes']['add_attributes'] = array(
    '#name' => 'add_attributes',
    '#type' => 'submit',
    '#value' => t('Add attributes'),
    '#submit' => array('add_attributes'),
    '#limit_validation_errors' => array(
      array('admin_attributes', 'attributes_fieldset', 'attributes')
    ),
  );
  //PART of Attributes end
  //PART of File start
  $form['admin_file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-admin_file')),
    '#group' => 'adminform',
  );

  $form['admin_file']['file_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name for file input.'),
    '#default_value' => variable_get('genericform_file_name'),
    '#required' => TRUE,
  );

  $file_options = array(
    'jpg' => 'IMAGE/JPG',
    'jpeg' => 'IMAGE/JPEG',
    'gif' => 'IMAGE/GIF',
    'png' => 'IMAGE/PNG',
    'pdf' => 'PDF',
    'doc' => 'MS Office Word/DOC',
    'docx' => 'MS Office Word/DOCX',
    'xls' => 'MS Office Excel/XLS',
    'xlsx' => 'MS Office Excel/XLSX',
  );

  $form['admin_file']['file_ext'] = array(
    '#type' => 'select',
    '#title' => t('Extensions'),
    '#description' => t('Choose extensions alowed for upload.'),
    '#options' => $file_options,
    '#multiple' => TRUE,
    '#default_value' => explode(' ', variable_get('genericform_file_ext')),
    '#required' => TRUE,
  );

  $form['admin_file']['add_file'] = array(
    '#name' => 'add_file',
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('save_file'),
    '#limit_validation_errors' => array(
      array('admin_file', 'file_name'),
      array('admin_file', 'file_ext')
    ),
  );
  //PART of Files end
  //PART of Sort start
  $form['admin_sort'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sort and pagination'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-admin_sort')),
    '#group' => 'adminform',
  );

  $attributes = get_attributes();
  $attributes_list = array();

  foreach ($attributes as $attribute) {
    $attributes_list[$attribute->attribute_name] = $attribute->attribute_name;
  }

  $form['admin_sort']['sort_by'] = array(
    '#type' => 'select',
    '#title' => t('Sort by'),
    '#options' => $attributes_list,
    '#description' => 'Choose this for sort item by one attribute.',
    '#empty_option' => t('- Select -'),
    '#default_value' => variable_get('genericform_sort_by'),
    '#required' => TRUE,
  );

  $limit_list = array();
  for($i = 1; $i <= 5; $i++){
    $limit_list[$i] = $i;
  }
  for($i = 5; $i <= 50; $i= $i + 5){
    $limit_list[$i] = $i;
  }
  $form['admin_sort']['limit'] = array(
    '#type' => 'select',
    '#title' => t('Limit'),
    '#options' => $limit_list,
    '#description' => 'Choose this for limit number item per page.',
    '#empty_option' => t('- Select -'),
    '#default_value' => variable_get('genericform_limit'),
    '#required' => TRUE,
  );

  $form['admin_sort']['add_sort'] = array(
    '#name' => 'add_sort',
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('save_sort'),
    '#limit_validation_errors' => array(
      array('admin_sort', 'sort_by'),
      array('admin_sort', 'limit'),
    ),
  );
  //PART of Sort end
  return $form;
}

/**
* attributes_add_more_add_one().
*
* This function add more one attribute input when (+) clicked.
*/
function attributes_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['num_attributes'])) {
    $form_state['num_attributes'] = 0;
    $form_state['num_attributes']++;
  }
  $form_state['num_attributes']++;
  $form_state['rebuild'] = TRUE;
}

/**
* attributes_add_more_del_one().
*
* This function del more one attribute input when (-) clicked.
*/
function attributes_add_more_del_one($form, &$form_state) {
  if (!isset($form_state['num_attributes'])) {
    $form_state['num_attributes'] = 0;
    $form_state['num_attributes']--;
  }
  $form_state['num_attributes']--;
  $form_state['rebuild'] = TRUE;
}

/**
* attributes_add_more_callback().
*
* This function return fieldset.
*/
function attributes_add_more_callback($form, $form_state) {
  return $form['admin_attributes']['attributes_fieldset'];
}
