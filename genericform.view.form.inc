<?php
/**
* @file
*
* View form
*
* This form provides user ability to see data and to sort that data.
*
*/
function view_form($form, $form_state){
  $header = array(
    array('data' => t('Image'), 'class' => array('genericform-image')),
    array(
      'data' => t('SORT BY ' . variable_get('genericform_sort_by')),
      'field' => 'sort',
      'class' => array('genericform-attribute')),
    array('data' => t('File'), 'class' => array('genericform-file')),
  );

  if (user_access('access genericform content')){
    $header[] = array('data' => t('Operations'));
  }

  $form = array();
  $form['#tree'] = TRUE;

  $class_name_var = variable_get('genericform_class_name');
  $item_name_var = variable_get('genericform_item_name');
  $limit_var = variable_get('genericform_limit');

  //PART of Filter start
  $form['view_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
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

  if (isset($form_state['values']['view_filter']['dropdown_first'])) {
    $first = $form_state['values']['view_filter']['dropdown_first'];
  }else {
     $first = key($class_list);
  }

  $form['view_filter']['dropdown_first'] = array(
    '#type' => 'select',
    '#title' => $class_name_var,
    '#option' => '- Select -',
    '#options' => $class_list,
    '#default_value' => $first,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'filter_callback',
      'wrapper' => 'dropdown_second_replace',
    ),
  );

  $items = get_items($first);
  $items_list = array();
  $items_list[0] = '- Select -';
  foreach ($items as $i) {
    $items_list[$i->item_id] = $i->item_name;
  }

  $second = 0;
  if (isset($form_state['values']['view_filter']['dropdown_second'])) {
     $second = $form_state['values']['view_filter']['dropdown_second'];
  }

  $form['view_filter']['dropdown_second'] = array(
    '#type' => 'select',
    '#title' => t($item_name_var),
    '#prefix' => '<div id="dropdown_second_replace">',
    '#suffix' => '</div>',
    '#options' => $items_list,
    '#default_value' => $second,
  );

  $form['view_filter']['filter'] = array(
    '#name' => 'filter',
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('filter_table_callback'),
  );
  //PART of Filter end
  //PART of Table start
  $sort = 'ASC';
  if (isset($_GET['sort']) && $_GET['sort'] == 'desc') {
    $sort = 'DESC';
  }
  $offset = '0';
  variable_set('genericform_page', '0');
  if (isset($_GET['page'])) {
    $offset = $_GET['page'];
    variable_set('genericform_page', $offset);
  }
  $items_all = get_items_all($sort, $offset);
  $rows = array();
  foreach ($items_all as $item) {
    $items_attributes = get_items_attributes($item->item_id);
    $attributes = '';
    foreach ($items_attributes as $attribute) {
      $attribute_options = array(
        'name' => $attribute->attribute_name,
        'value' => $attribute->attribute_value,
      );
      $attributes .= theme('attribute', $attribute_options);
    }

    $items_files = get_items_files($item->item_id);

    $images = '';
    $files = '';
    foreach ($items_files as $file) {
      $image_type = array('image/jpeg', 'image/jpg', 'image/gif', 'image/png',);
      if (in_array($file->filemime, $image_type)) {
        $image_path = file_create_url($file->uri);
        $image_options = array(
          'path' => $image_path,
          'alt' => 'Test alt',
          'title' => 'Test title',
          'width' => '100%',
        );
        $images .= theme('image', $image_options);
      }else {
        $file_path = file_create_url($file->uri);
        $download_options = array(
          'name' => $file->filename,
          'path' => $file_path,
        );
        $files .= theme('download', $download_options);
      }
    }

    if (user_access('access genericform content')) {
      $rows[] = array(
        array('data' => $images, 'align' => 'center',),
        $attributes,
        $files,
        l('Edit', 'genericform/edit', array('query' => array('id' => $item->item_id),'html' => TRUE)),
      );
    }else{
      $rows[] = array(
        array('data' => $images, 'align' => 'center',),
        $attributes,
        $files,
      );
    }
  }
  $form['table'] = array(
    '#name' => 'table',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('Table has no row!'),
    '#prefix' => '<div id="filter-table-wrapper">',
    '#suffix' => '</div>',
  );

  $page = pager_default_initialize(get_item_id_sort_by_attribute_count(), $limit_var);
  $form['pager'] = array('#markup' => theme('pager'));
  //PART of Table end

  return $form;
}

/**
* filter_callback().
*
* This function return fieldset with items filtered by class.
*/
function filter_callback($form, &$form_state) {
    return $form['view_filter']['dropdown_second'];
}

/**
* filter_table_callback().
*
* This function return table with items filtered by class and item id.
*/
function filter_table_callback($form, &$form_state) {
    $first = $form_state['values']['view_filter']['dropdown_first'];
    $second = $form_state['values']['view_filter']['dropdown_second'];
    variable_set('genericform_class', $first);
    variable_set('genericform_item', $second);

    $form_state['rebuild'] = TRUE;
    if(isset($_GET['page'])) {
      drupal_goto('genericform/view');
    }

    return $form['table'];
}
