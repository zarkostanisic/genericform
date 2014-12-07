<?php
/**
* @file
*
* Edit form
*
* This form provides editerator ability to edit data.
*
*/
function edit_form($form, $form_state, $no_js_use = FALSE){
  $form = array();
  $form['#tree'] = TRUE;
  $form['#classes']['enctype'] = 'multipart/form-data';

  $form['editform'] = array(
    '#type' => 'vertical_tabs',
  );

  if(!isset($_SESSION['genericform_item_id'])) {
    $_SESSION['genericform_item_id'] = $_GET['id'];
  }else if(isset($_GET['id']) && $_SESSION['genericform_item_id'] != $_GET['id']){
    $_SESSION['genericform_item_id'] = $_GET['id'];
  }

  $id = $_SESSION['genericform_item_id'];

  $item = get_item($id);

  $class_name_var = variable_get('genericform_class_name');
  $item_name_var = variable_get('genericform_item_name');

  $file_ext_var = variable_get('genericform_file_ext');

  //PART of items start
  $form['edit_item'] = array(
    '#type' => 'fieldset',
    '#title' => t($item_name_var),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-edit_items')),
    '#group' => 'editform'
  );

  $class = get_classes();
  $class_list = array();

  foreach ($class as $c) {
    $class_list[$c->class_id] = $c->class_name;
  }

  $form['edit_item']['class'] = array(
    '#type' => 'select',
    '#title' => t($class_name_var),
    '#options' => $class_list,
    '#description' => t('Choose ' . strtolower($class_name_var) . ' for this ' . strtolower($item_name_var) . '.'),
    '#default_value' => $item->class_id,
  );

  $form['edit_item']['item_name'] = array(
    '#type' => 'textfield',
    '#title' => t($item_name_var),
    '#description' => t('Field for ' . strtolower($item_name_var) . ' name. e.g. Sparrow, Mercedes Vito, Asus x5dab.'),
    '#default_value' => $item->item_name,
    '#required' => TRUE,
  );

  $form['edit_item']['item_id'] = array(
    '#type' => 'hidden',
    '#value' => $item->item_id,
  );

  $form['edit_item']['edit_item_basic'] = array(
    '#name' => 'edit_item_basic',
    '#type' => 'submit',
    '#value' => t('Edit'),
    '#submit' => array('edit_item_basic'),
    '#limit_validation_errors' => array(
        array('edit_item', 'item_name'),
        array('edit_item', 'item_id'),
        array('edit_item', 'class'),
    ),
  );
  //PART of items end
  //PART of Items attributes start

  $form['edit_item_attributes'] = array(
    '#type' => 'fieldset',
    '#title' => t($item_name_var . ' details'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-view_filter')),
    '#group' => 'editform'
  );


  $attributes = get_items_attributes($id);

  $i = 0;
  foreach ($attributes as $attribute){
    $form['edit_item_attributes']['attributes'][$i] = array(
      '#type' => 'textfield',
      '#title' => $attribute->attribute_name,
      '#description' => 'Field for ' . strtolower($item_name_var)  . ' attribute ' . strtolower($attribute->attribute_name) . '.',
      '#attributes' => array('multiple' => 'multiple'),
      '#default_value' => $attribute->attribute_value,
      '#required' => TRUE,
    );

    $form['edit_item_attributes']['attributes_id'][$i] = array(
      '#type' => 'hidden',
      '#value' => $attribute->attribute_id,
    );

    $i++;
  }

  $form['edit_item_attributes']['edit_item_details'] = array(
    '#name' => 'edit_item_details',
    '#type' => 'submit',
    '#value' => t('Edit details'),
    '#submit' => array('edit_item_details'),
    '#limit_validation_errors' => array(
        array('edit_item_attributes', 'attributes'),
        array('edit_item_attributes', 'attributes_id'),
        array('edit_item', 'item_id'),
    ),
  );
  //PART of Items attributes end
  //PART of Items files start
  $form['edit_item_files'] = array(
    '#type' => 'fieldset',
    '#title' => t($item_name_var . ' files'),
    '#collapsible' => TRUE,
    '#group' => 'editform'
  );

  $form['edit_item_files']['link'] = array(
    '#markup' =>
      l(t('Manage exixsting files for ' . strtolower($item_name_var) . '.'), 'genericform/managed_files',
        array('query' => array('id' => $item->item_id), 'html' => TRUE)),
  );

  $form['edit_item_files']['files_fieldset'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="files-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_files'])) {
    $form_state['num_files'] = 1;
  }

  for ($i = 0; $i < $form_state['num_files']; $i++) {
    $form['edit_item_files']['files_fieldset']['files'][$i] = array(
      '#type'   => "managed_file",
      '#title'  => t(variable_get('genericform_file_name')),
      '#description' => t("Only " . $file_ext_var . " allowed for upload."),
      '#upload_location'    => "public://genericform/files/",
      '#upload_validators'  =>
        array("file_validate_extensions" =>
          array(variable_get('genericform_file_ext'))),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'files_fix',
      ),
    );
  }

  $form['edit_item_files']['files_fieldset']['add_files'] = array(
    '#name' => 'add_files',
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('files_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'files_add_more_callback',
      'wrapper' => 'files-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(
      array('edit_item_files', 'files_fieldset', 'files'),
    ),
  );

  if ($form_state['num_files'] > 1) {
    $form['edit_item_files']['files_fieldset']['del_files'] = array(
      '#name' => 'del_files',
      '#type' => 'submit',
      '#value' => t('-'),
      '#submit' => array('files_add_more_del_one'),
      '#ajax' => array(
        'callback' => 'files_add_more_callback',
        'wrapper' => 'files-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }

  $form['edit_item_files']['add_edit_files'] = array(
    '#name' => 'add_edit_files',
    '#type' => 'submit',
    '#value' => t('Add files'),
    '#submit' => array('add_edit_files'),
    '#limit_validation_errors' => array(
      array('edit_item_files', 'files_fieldset', 'files'),
      array('edit_item', 'item_id'),
    ),
  );

  //PART of Items files end


  return $form;
}

/**
* files_add_more_add_one().
*
* This function add more one file input when (+) clicked.
*/
function files_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['num_files'])) {
    $form_state['num_files'] = 0;
    $form_state['num_files']++;
  }

  $form_state['num_files']++;
  $form_state['rebuild'] = TRUE;
}

/**
* files_add_more_del_one().
*
* This function del more one file input when (-) clicked.
*/
function files_add_more_del_one($form, &$form_state) {
  if (!isset($form_state['num_files'])) {
    $form_state['num_files'] = 0;
    $form_state['num_files']--;
  }

  $form_state['num_files']--;
  $form_state['rebuild'] = TRUE;
}

/**
* files_add_more_callback().
*
* This function return fieldset
*/
function files_add_more_callback($form, $form_state) {
  return $form['edit_item_files']['files_fieldset'];
}

function files_fix() {
 $_SESSION['genericform_item_id'] = $form_state['values']['edit_item']['item_id'];
}
