<?php
/**
 * @file
 * This is the file description for Snippet Editor.Admin module.
 */

/**
 * Menu page callback: builds the page for administering snippets.
 */
function snippet_list() {
	
	$header = array(t('Snippet'), t('Operations'));
	$rows = array();
	
  foreach (snippet_block::load_all() as $sb) {
		$machine_name = _snippet_editor_machine_name($sb->file->filename);
		$row  = array($machine_name);
		
		if(user_access(_snippet_editor_permission_name($sb->file->filename))) {
			$row[] = l(t('edit snippet'), 'admin/structure/snippets/' . $sb->module . '/' . $machine_name . '/edit');
		}
		else {
			$row[] = '&nbsp;';
		}
		
		$rows[] = $row;
	}
	
	return theme('table', array('header' => $header, 'rows' => $rows));
}

function snippet_edit_form($form, &$form_state, $module, $snippet_machine_name) {
	
	$sb = _snippet_editor_get_snippet_object_for_filename($module, $snippet_machine_name);	
	$form['snippet'] = array(
    '#type' => 'textarea',		
    // '#type' => 'text_format',
    '#title' => t('Snippet source'),
    // '#format' => NULL,
    '#weight' => 0,	
		'#rows' => 20,
		'#default_value' => file_get_contents($sb->file->uri),
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save Changes',
	);
	
	return $form;		
}

function snippet_edit_form_validate($form, &$form_state) {
	if($form_state['values']['snippet'] == '') {
    form_set_error('', t('Source field cannot be empty.'));		
	}
}

function snippet_edit_form_submit($form, &$form_state) {
	$module = $form_state['build_info']['args'][0];
	$snippet_machine_name = $form_state['build_info']['args'][1];	
	$sb = _snippet_editor_get_snippet_object_for_filename($module, $snippet_machine_name);	
	
	if($sb) {
		if(file_put_contents($sb->file->uri, $form_state['values']['snippet']) != FALSE) {
			drupal_set_message(t('The changes to the snippet <em>' . $snippet_machine_name . '</em> have been succuessfully saved.'));
		}
		else {
    	form_set_error('', t('Unable to save the changes to the file. Make sure the file has write permissions.'));				
		}
	}
	else {
    form_set_error('', t('Unable to load snippet for saving.'));				
	}
}