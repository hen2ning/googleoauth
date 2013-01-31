<?php

function googleoauth_settings() {
    $library = libraries_get_path('google-api');
    if (!file_exists($library . '/src/Google_Client.php')) {
        drupal_set_message(t('Download the  Google Api Client library into :path, using this link :url', array(':path' => $library, ':url' => GOOGLE_API_DOWNLOAD_URI)), 'warning');
    }
    $form['googleoauth_title_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Title of login link'),
        '#default_value' => variable_get('googleoauth_title_link', ''),
    );

    $form['googleoauth_console'] = array(
        '#title' => t('Google API Client'),
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#description' => t('Get from <a href="https://code.google.com/apis/console/#:overview:access" target="_blank">https://code.google.com/apis/console/#:overview:access</a>'),
    );

    $form['googleoauth_console']['googleoauth_application_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Application name'),
        '#default_value' => variable_get('googleoauth_application_name', ''),
    );
    $form['googleoauth_console']['googleoauth_oauth2_client_id'] = array(
        '#required' => TRUE,
        '#type' => 'textfield',
        '#title' => t('Client ID'),
        '#default_value' => variable_get('googleoauth_oauth2_client_id', ''),
    );
    $form['googleoauth_console']['googleoauth_oauth2_client_secret'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => t('Client secret'),
        '#default_value' => variable_get('googleoauth_oauth2_client_secret', ''),
    );
    $form['googleoauth_console']['googleoauth_developer_key'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => t('Google Developer API Key'),
        '#default_value' => variable_get('googleoauth_developer_key', ''),
    );
    $form['googleoauth_console']['googleoauth_redirect_uri'] = array(
        '#type' => 'textfield',
        '#disabled' => TRUE,
        '#title' => t('Redirect URI'),
        '#default_value' => 'http://' . $_SERVER['HTTP_HOST'] . '/googleoauth-callback',
        '#description' => t('IMPORTANT: You have to point this url as Redirect URIs for your application'),
    );
    //--------------------
    $form['googleoauth_settings'] = array(
        '#title' => t('Google API Client'),
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
    );

    $form['googleoauth_settings']['googleoauth_allowed_domains'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed domain names'),
        '#default_value' => variable_get('googleoauth_allowed_domains', ''),
        '#description' => t("Restrict Registration & Login only to the user email addresses of above domain names. Separated multiple domain names by new line. Ex: gmail.com"),
    );
    $form['googleoauth_settings']['googleoauth_block_domains'] = array(
        '#type' => 'checkbox',
        '#title' => t('Block Above Domain Names.'),
        '#default_value' => variable_get('googleoauth_block_domains', ''),
        '#description' => t("Instead of allowing above domain names, if you check this field above domain names will be blocked and all other will be accepted."),
    );
    $form['googleoauth_settings']['googleoauth_allow_registration'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow user registration using Google OAuth'),
        '#default_value' => variable_get('googleoauth_allow_registration', 0),
        '#description' => t("Instead of user registration settings, if you check this field users could register using Google OAuth"),
    );
    $form['googleoauth_settings']['googleoauth_onlygoogleauth'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow authentication only through Google account'),
        '#default_value' => variable_get('googleoauth_onlygoogleauth', 0),
        '#description' => t("If checked, user could login in only through Google account. IMPORTANT: Flush the cache"),
    );
    $form['googleoauth_settings']['googleoauth_hidden_pass'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide password field in user settings form'),
        '#default_value' => variable_get('googleoauth_hidden_pass', 0),
//        '#description' => t("Instead of user registration settings, if you check this field users could register using Google OAuth"),
    );
    return system_settings_form($form);
}