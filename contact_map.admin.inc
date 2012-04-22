<?php

function contact_map_settings() {
    return drupal_get_form('contact_map_settings_form');
}

function contact_map_settings_form($form, $form_state) {
    
    for ($i = 1; $i < 18; $i++) {
        $zoom[$i] = $i;
    }
    
    $form['type'] = array(
        '#type' => 'select',
        '#title' => t('Provider of Map'),
        '#options' => array(
            'yandex' => t('Yandex Map'),
            '2gis' => t('2Gis')
        ),
        '#default_value' => variable_get('contact_map_provider', 'yandex')
    );
    $form['center']['right'] = array(
      '#type' => 'textfield',
      '#title' => 'Координаты центра (Первая цифра)',
      '#default_value' => variable_get('contact_map_cc_right', '52.2'),
      '#description' => t('Like this 52.2'),
      '#required' => TRUE
    );
    
    $form['center']['left'] = array(
      '#type' => 'textfield',
      '#title' => 'Координаты центра (Вторая цифра)',
      '#default_value' => variable_get('contact_map_cc_left', '104.2'),
      '#description' => t('Like this 104.2'),
      '#required' => TRUE
    );
    
   
    
    $form['zoom'] = array(
        '#type' => 'select',
        '#title' => t('Zoom'),
        '#default_value' => variable_get('contact_map_zoom', '16'),
        '#options' => $zoom,
        '#description' => 'for 2gis from 1 till 17',
        '#required' => TRUE
    );
    
    $form['sizes'] = array(
		'#type' => 'fieldset',
		'#title' => 'Размеры',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
    
    $form['sizes']['min_width'] = array(
        '#type' => 'textfield',
        '#title' => 'Мнимальная ширина, px',
        '#default_value' => variable_get('contact_map_min_width', '500'),
        '#description' => 'Минимальная или стандартная высота',
        '#required' => TRUE
    );
    $form['sizes']['min_height'] = array(
        '#type' => 'textfield',
        '#title' => 'Мнимальная высота, px',
        '#default_value' => variable_get('contact_map_min_height', '350'),
        '#description' => 'Минимальная или стандартная высота',
        '#required' => TRUE
    );
    $form['sizes']['max_width'] = array(
        '#type' => 'textfield',
        '#title' => 'Максимальная ширина, px',
        '#default_value' => variable_get('contact_map_max_width', '700'),
        '#description' => 'Максимальная ширина',
        '#required' => TRUE
    );
    $form['sizes']['max_height'] = array(
        '#type' => 'textfield',
        '#title' => 'Мнимальная высота, px',
        '#default_value' => variable_get('contact_map_max_height', '500'),
        '#description' => 'Максимальная высота',
        '#required' => TRUE
    );
    $form['sizes']['button'] = array(
        '#type' => 'checkbox',
        '#title' => 'Кнопка увеличения карты ',
        '#default_value' => variable_get('contact_map_button', '1'),
        '#description' => 'Показывать кнопку увеличения карты',
       
    );
    
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save')
    );
   
    drupal_add_js('http://api-maps.yandex.ru/2.0/?load=package.full&lang=ru-RU', 'file');
        
        $code = "
         ymaps.ready(function () { 
            var myMap = new ymaps.Map('myMap', {
                // центр и коэффициент масштабирования однозначно
                // определяют область картографирования
                center: [" . variable_get('contact_map_cc_ymaps', '52.2, 104.2') . "],
                zoom: " . variable_get('contact_map_zoom', '16') . ",
                
                behaviors: ['default', 'scrollZoom']
            });
            
            // Добавление стандартного набора кнопок
            
            myMap.controls.add('mapTools')
                // Добавление кнопки изменения масштаба 
                .add('zoomControl')
                // Добавление списка типов карты
                 .add('typeSelector');
                 
            /* При щелчке на карте показывается балун со
                значениями координат в месте клика */
             myMap.events.add('click',
                 function(e) {
                     myMap.balloon.open(
                         // Позиция балуна
                         e.get('coordPosition'), {
                             // Свойства балуна:
                             // контент балуна
                             contentBody: 'Значение: ' +
                             e.get('coordPosition')
                         }   
                     )
                 }
             );     
            // Включаем поведения после создания карты
            myMap.behaviors.enable('drag');
            
            var myGeoObject = new ymaps.GeoObject({
              // Тип геометрии - точка.
              type: 'Point',
              // Координаты точки.
              coordinates: [55.8, 37.8]
            });
            
            myMap.geoObjects.add(myGeoObject);
            
        })
        ";
        
    drupal_add_js($code, 'inline');
    
    $form['map'] = array(
        '#markup' =>  '<h2>На карте можно получить координаты нужного места</h2><div id="myMap" style="width:'. variable_get('contact_map_min_width', '500') .'px; height:'. variable_get('contact_map_min_height', '350') .'px;" class="default"></div>'  
    );
    
    
    return $form;
}

function contact_map_settings_form_Submit($form, &$form_state) {
    $values = $form_state['values']; 
    variable_set('contact_map_provider',  $values['type']);
    variable_set('contact_map_cc_left',  $values['left']);
    variable_set('contact_map_cc_right',  $values['right']);
    
    variable_set('contact_map_cc_2gis',  $values['left'] . "," .$values['right']);
    variable_set('contact_map_cc_ymaps',  $values['right'] . "," . $values['left']);
    
    variable_set('contact_map_zoom',  $values['zoom']);
    variable_set('contact_map_min_width',  $values['min_width']);
    variable_set('contact_map_min_height',  $values['min_height']);
    variable_set('contact_map_max_width',  $values['max_width']);
    variable_set('contact_map_max_height',  $values['max_height']);
    variable_set('contact_map_button',  $values['button']);
    
    drupal_set_message(t('Saved'));
}


function contact_map_settings_points() {
    $header = array(
		'name' => array('data' => t('Name')),
		'content' => array('data' => t('Content')),
		'left' => array('data' => t('First Coord')),
        'right' => array('data' => t('Second Coord')),
		'options' => array('data' => t('Actions')),
	);

	$query = db_select('contact_map', 'cm');

	$count_query = clone $query;
	$count_query->addExpression('COUNT(cm.pid)');

	$query = $query->extend('PagerDefault')->extend('TableSort');
	$query->fields('cm')
			->limit(20)
			->orderByHeader($header)
			->setCountQuery($count_query);
            
	$result = $query->execute();

	$rows = array();
	foreach ($result as $item) {
		$del = l('Изменить', "admin/contact_map/points/edit/$item->pid") . ' ' . l('Удалить', "admin/contact_map/points/delete/$item->pid");

		$rows[] = array(
			'class' => array('feedback-row-' . $item->pid),
			'data' => array(
				$item->name,
				$item->content,
				$item->left_,
				$item->right_,
				$del
			),
		);
	}

	if($result->rowCount()){
		$rows[] = array(
			'data' => array(
				array(
					'data' => theme('pager'),
					'colspan' => 5,
				),
			),
		);
	}

	$element = array(
		'#theme' => 'table',
		'#rows' => $rows,
		'#header' => $header,
		'#empty' => 'Нет записей',
		'#attributes' => array('class' => array('content_map-table'))
	);

	return $element;
}


function contact_map_points_form($form, $form_state, $data = NULL) {
    $form['pid'] = array(
        '#type' => 'hidden',
        '#value' => ($data) ? $data->pid : ''
    );
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => ($data) ? $data->name : '',
        '#required' => TRUE
    );
    $form['content'] = array(
        '#type' => 'textarea',
        '#title' => t('Content'),
        '#default_value' => ($data) ? $data->content : '',
        '#required' => TRUE
    );
    $form['left'] = array(
        '#type' => 'textfield',
        '#title' => t('First coord'),
        '#default_value' => ($data) ? $data->content : '',
        '#required' => TRUE
    );
    $form['right'] = array(
        '#type' => 'textfield',
        '#title' => t('Second Coord'),
        '#default_value' => ($data) ? $data->content : '',
        '#required' => TRUE
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit')
    );
    
    drupal_add_js('http://api-maps.yandex.ru/2.0/?load=package.full&lang=ru-RU', 'file');
        
    $code = "
         ymaps.ready(function () { 
            var myMap = new ymaps.Map('myMap', {
                // центр и коэффициент масштабирования однозначно
                // определяют область картографирования
                center: [" . variable_get('contact_map_cc_ymaps', '52.2, 104.2') . "],
                zoom: " . variable_get('contact_map_zoom', '16') . ",
                
                behaviors: ['default', 'scrollZoom']
            });
            
            // Добавление стандартного набора кнопок
            
            myMap.controls.add('mapTools')
                // Добавление кнопки изменения масштаба 
                .add('zoomControl')
                // Добавление списка типов карты
                 .add('typeSelector');
                 
            /* При щелчке на карте показывается балун со
                значениями координат в месте клика */
             myMap.events.add('click',
                 function(e) {
                     myMap.balloon.open(
                         // Позиция балуна
                         e.get('coordPosition'), {
                             // Свойства балуна:
                             // контент балуна
                             contentBody: 'Значение: ' +
                             e.get('coordPosition')
                         }   
                     )
                 }
             );     
            // Включаем поведения после создания карты
            myMap.behaviors.enable('drag');
            
            var myGeoObject = new ymaps.GeoObject({
              // Тип геометрии - точка.
              type: 'Point',
              // Координаты точки.
              coordinates: [55.8, 37.8]
            });
            
            myMap.geoObjects.add(myGeoObject);
            
        })
        ";
        
    drupal_add_js($code, 'inline');
    
    $form['map'] = array(
        '#markup' =>  '<h2>На карте можно получить координаты нужного места</h2><div id="myMap" style="width:'. variable_get('contact_map_min_width', '500') .'px; height:'. variable_get('contact_map_min_height', '350') .'px;" class="default"></div>'  
    );
    return $form;
}

function contact_map_points_form_submit($form, &$form_state) {
    $values = $form_state['values'];
        $fields = array(
            'name' => $values['name'],
            'content' => $values['content'],
            'left_' => $values['left'],
            'right_' => $values['right']
        );
    if(!empty($values['pid'])) {
        db_update('contact_map')->condition('pid', $values['pid'], '=')->fields($fields)->execute();
    } else {
        db_insert('contact_map')->fields($fields)->execute();
    }
     drupal_set_message(t('Saved'));
    $form_state['redirect'] = 'admin/config/content/contact_map/points';
}

function contact_map_settings_points_add() {
    return drupal_get_form('contact_map_points_form');
}

function contact_map_settings_points_edit($pid) {
    $query = db_select('contact_map', 'cm');
    $query->condition('pid', $pid, '=');
    $query->fields('cm');
    $result = $query->execute()->fetchObject();
    return drupal_get_form('contact_map_points_form', $result);
}

function contact_map_settings_points_delete($form, $form_state, $pid) {
	$form['pid'] = array(
		'#type' => 'hidden',
		'#value' => $pid,
	);
	$confirm = confirm_form($form, t('Вы действительно хотите удалить эту метку ?'), 'admin/config/content/contact_map/points', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
	return $confirm;
}

function contact_map_settings_points_delete_submit($form, &$form_state) {
    $pid = $form_state['values']['pid'];
	
	db_delete('contact_map')->condition('pid', $pid, '=')->execute();
	
	$form_state['redirect'] = 'admin/config/content/contact_map/points';
}