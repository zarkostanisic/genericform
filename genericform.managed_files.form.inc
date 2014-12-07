<?php
/**
* @file
*
* Managed Files form
*
* This form provides moderator ability to managed files for item.
*
*/
function managed_files_form($form, $form_state){
  $header = array(
    array('data' => t('Id'),),
    array('data' => t('File'), 'class' => array('genericform-image')),
    array('data' => t('Type'), 'class' => array('genericform-managed')),
    array('data' => t('Size')),
  );

  $form = array();
  $form['#tree'] = TRUE;

  //PART of Table start
  $id = $_GET['id'];

  $items_files = get_items_files($id);

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
      $rows[$file->fid] = array(
        array('data' => $file->fid),
        array('data' => theme('image', $image_options)),
        array('data' => strtoupper($file->filemime), 'class' => array('genericform-managed')),
        array('data' => format_size($file->filesize)),
      );
    }else {
      $file_path = file_create_url($file->uri);
      $download_options = array(
        'name' => $file->filename,
        'path' => $file_path,
      );

      $rows[$file->fid] = array(
        array('data' => $file->fid),
        array('data' => theme('download', $download_options)),
        array('data' => strtoupper($file->filemime), 'class' => array('genericform-managed')),
        array('data' => format_size($file->filesize)),
      );
    }
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No content available.'),
  );

  $form['del_managed_files'] = array(
    '#name' => 'del_managed_files',
    '#type' => 'submit',
    '#value' => t('Delete files'),
    '#submit' => array('del_managed_files'),
  );

  //PART of Table end

  return $form;
}

