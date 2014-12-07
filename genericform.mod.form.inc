<?php
/**
* @file
*
* Mod form
*
* This form provides moderator ability to insert data.
*
*/
function mod_form($form, $form_state, $no_js_use = FALSE){
  $form = array();
  $form['#tree'] = TRUE;
  $form['#classes']['enctype'] = 'multipart/form-data';

  $class_name_var = variable_get('genericform_class_name');
  $class_fieldset = variable_get('genericform_class_fieldset');

  $item_name_var = variable_get('genericform_item_name');
  $item_fieldset = variable_get('genericform_item_fieldset');

  $file_ext_var = variable_get('genericform_file_ext');

  $form['modform'] = array(
    '#type' => 'vertical_tabs',
  );
  //PART of class start
  $form['mod_class'] = array(
    '#type' => 'fieldset',
    '#title' => t($class_fieldset),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-mod_class')),
    '#group' => 'modform'
  );

  $form['mod_class']['class_fieldset'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="class-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_class'])) {
    $form_state['num_class'] = 1;
  }

  for ($i = 0; $i < $form_state['num_class']; $i++) {
    $form['mod_class']['class_fieldset']['class'][$i] = array(
      '#type' => 'textfield',
      '#title' => t($class_name_var),
      '#description' => t('Field for ' . strtolower($class_name_var) . ' name. e.g. Birds, Mercedes, Laptops.'),
      '#class' => array('multiple' => 'multiple'),
      '#required' => TRUE,
    );
  }

  $form['mod_class']['class_fieldset']['add_class'] = array(
    '#name' => 'add_class',
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('class_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'class_add_more_callback',
      'wrapper' => 'class-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(
        array('mod_class', 'class_fieldset', 'class'),
    ),
  );

   if ($form_state['num_class'] > 1) {
    $form['mod_class']['class_fieldset']['del_class'] = array(
      '#name' => 'del_class',
      '#type' => 'submit',
      '#value' => t('-'),
      '#submit' => array('class_add_more_del_one'),
      '#ajax' => array(
        'callback' => 'class_add_more_callback',
        'wrapper' => 'class-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }

  $form['mod_class']['add_classes'] = array(
    '#type' => 'submit',
    '#value' => t('Add ' . strtolower($class_fieldset)),
    '#submit' => array('add_classes'),
    '#limit_validation_errors' => array(
      array('mod_class', 'class_fieldset', 'class'),
    ),
  );
  //PART of class end
  //PART of items start
  $form['mod_items'] = array(
    '#type' => 'fieldset',
    '#title' => t($item_fieldset),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-mod_items')),
    '#group' => 'modform',
  );

  $class = get_classes();
  $class_list = array();

  foreach ($class as $c) {
    $class_list[$c->class_id] = $c->class_name;
  }

  $form['mod_items']['class'] = array(
    '#type' => 'select',
    '#title' => t($class_name_var),
    '#options' => $class_list,
    '#description' => t('Choose ' . strtolower($class_name_var) . ' for this ' . strtolower($item_name_var) . '.'),
    '#required' => TRUE,
  );

  $form['mod_items']['items_fieldset'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="items-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_items'])) {
    $form_state['num_items'] = 1;
  }

  for ($i = 0; $i < $form_state['num_items']; $i++) {
    $form['mod_items']['items_fieldset']['items'][$i] = array(
      '#type' => 'textfield',
      '#title' => t($item_name_var),
      '#description' => t('Field for ' . strtolower($item_name_var) . ' name. e.g. Sparrow, Mercedes Vito, Asus x5dab.'),
      '#items' => array('multiple' => 'multiple'),
      '#required' => TRUE,
    );
  }

  $form['mod_items']['items_fieldset']['add_item'] = array(
  	'#name' => 'add_item',
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('items_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'items_add_more_callback',
      'wrapper' => 'items-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(
      array('mod_items', 'items_fieldset', 'items'),
    ),
  );

  if ($form_state['num_items'] > 1) {
    $form['mod_items']['items_fieldset']['del_item'] = array(
      '#name' => 'del_item',
      '#type' => 'submit',
      '#value' => t('-'),
      '#submit' => array('items_add_more_del_one'),
      '#ajax' => array(
        'callback' => 'items_add_more_callback',
        'wrapper' => 'items-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }

  $form['mod_items']['add_items'] = array(
    '#type' => 'submit',
    '#value' => t('Add ' . strtolower($item_fieldset)),
    '#submit' => array('add_items'),
    '#limit_validation_errors' => array(
        array('mod_items', 'class'),
        array('mod_items', 'items_fieldset', 'items'),
    ),
  );
  //PART of items end
  //PART of Items attributes start
  $form['mod_item_attributes'] = array(
    '#type' => 'fieldset',
    '#title' => t($item_name_var . ' details'),
    '#collapsible' => TRUE,
    '#attributes' => array('id' => array('form-view_filter')),
    '#group' => 'modform',
  );

  $form['mod_item_attributes']['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#prefix' => '<div id="form-view_filter">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('container-inline')),
  );

  $class = get_classes();
  $class_list = array();
  $class_list[0] = '- Select -';
  foreach ($class as $c) {
    $class_list[$c->class_id] = $c->class_name;
  }

  $value_dropdown_first = '';
  if (isset($form_state['values']['mod_item_attributes']['filter'])) {
    $filter = $form_state['values']['mod_item_attributes']['filter'];
    $value_dropdown_first = $filter['dropdown_first'];
  }else {
     $value_dropdown_first = key($class_list);
  }

  $form['mod_item_attributes']['filter']['dropdown_first'] = array(
    '#type' => 'select',
    '#title' => t(ucfirst($class_name_var)),
    '#options' => $class_list,
    '#default_value' => $value_dropdown_first,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'filter_callback',
      'wrapper' => 'dropdown_second_replace',
    ),
  );

  $items = get_items_add($value_dropdown_first);
  $items_list = array();

  foreach ($items as $i) {
    $items_list[$i->item_id] = $i->item_name;
  }

  $value_dropdown_second = '';
  if(isset($form_state['values']['mod_item_attributes']['filter'])){
    $filter = $form_state['values']['mod_item_attributes']['filter'];
     $value_dropdown_second = $filter['dropdown_second'];
  }

  $form['mod_item_attributes']['filter']['dropdown_second'] = array(
    '#type' => 'select',
    '#title' => t(ucfirst($item_name_var)),
    '#prefix' => '<div id="dropdown_second_replace">',
    '#suffix' => '</div>',
    '#options' => $items_list,
    '#required' => TRUE,
  );

  $attributes = get_attributes();

  $i = 0;
  foreach ($attributes as $attribute){
    $form['mod_item_attributes']['attributes'][$i] = array(
      '#type' => 'textfield',
      '#title' => $attribute->attribute_name,
      '#description' => 'Field for ' . strtolower($item_name_var)  . ' attribute ' . strtolower($attribute->attribute_name) . '.',
      '#attributes' => array('multiple' => 'multiple'),
      '#required' => TRUE,
    );

    $form['mod_item_attributes']['attributes_id'][$i] = array(
      '#type' => 'hidden',
      '#value' => $attribute->attribute_id,
    );
    $i++;
  }

  $form['mod_item_attributes']['files_fieldset'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="files-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_files'])) {
    $form_state['num_files'] = 1;
  }

  for ($i = 0; $i < $form_state['num_files']; $i++) {
    $form['mod_item_attributes']['files_fieldset']['files'][$i] = array(
      '#type'   => "managed_file",
      '#title'  => t(variable_get('genericform_file_name')),
      '#description' => t("Only " . $file_ext_var . " allowed for upload."),
      '#upload_location'    => "public://genericform/files/",
      '#upload_validators'  =>
        array("file_validate_extensions" =>
          array($file_ext_var)),
      '#required' => TRUE,
    );
  }

  $form['mod_item_attributes']['files_fieldset']['add_files'] = array(
    '#name' => 'add_files',
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('files_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'files_add_more_callback',
      'wrapper' => 'files-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(
      array('mod_item_attributes', 'files_fieldset', 'files'),
    ),
  );

  if ($form_state['num_files'] > 1) {
    $form['mod_item_attributes']['files_fieldset']['del_files'] = array(
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

  $form['mod_item_attributes']['add_items_details'] = array(
    '#name' => 'add_items_details',
    '#type' => 'submit',
    '#value' => t('Save details'),
    '#submit' => array('add_items_details'),
    '#limit_validation_errors' => array(
      array('mod_item_attributes', 'filter', 'dropdown_second'),
      array('mod_item_attributes', 'files_fieldset', 'files'),
      array('mod_item_attributes', 'attributes'),
      array('mod_item_attributes', 'attributes_id'),
    ),
  );
  //PART of Items attributes end

	return $form;
}

/**
* class_add_more_add_one().
*
* This function add more one class input when (+) clicked.
*/
function class_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['num_class'])) {
    $form_state['num_class'] = 0;
    $form_state['num_class']++;
  }

  $form_state['num_class']++;
  $form_state['rebuild'] = TRUE;
}

/**
* class_add_more_del_one().
*
* This function del more one class input when (-) clicked.
*/
function class_add_more_del_one($form, &$form_state) {
  if (!isset($form_state['num_class'])) {
    $form_state['num_class'] = 0;
    $form_state['num_class']--;
  }

  $form_state['num_class']--;
  $form_state['rebuild'] = TRUE;
}

/**
* class_add_more_callback().
*
* This function return fieldset
*/
function class_add_more_callback($form, $form_state) {
  return $form['mod_class']['class_fieldset'];
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
  return $form['mod_item_attributes']['files_fieldset'];
}

/**
* items_add_more_add_one().
*
* This function add more one item input when (+) clicked.
*/
function items_add_more_add_one($form, &$form_state) {
  if (!isset($form_state['num_items'])) {
    $form_state['num_items'] = 0;
    $form_state['num_items']++;
  }
  $form_state['num_items']++;
  $form_state['rebuild'] = TRUE;
}

/**
* items_add_more_del_one().
*
* This function del more one item input when (-) clicked.
*/
function items_add_more_del_one($form, &$form_state) {
  if (!isset($form_state['num_items'])) {
    $form_state['num_items'] = 0;
    $form_state['num_items']--;
  }
  $form_state['num_items']--;
  $form_state['rebuild'] = TRUE;
}

/**
* items_add_more_callback().
*
* This function return fieldset.
*/
function items_add_more_callback($form, $form_state) {
  $form_state['values']['mod_item_attributes']['filter']['dropdown_second'] = 0;
  $form_state['rebuild'] = TRUE;
  return $form['mod_items']['items_fieldset'];
}

/**
* items_add_more_callback().
*
* This function return fieldset with items filtered by class.
*/
function filter_callback($form, $form_state) {
    return $form['mod_item_attributes']['filter']['dropdown_second'];
}
