<?php


/**
 * Google OAuth callback page
 * @global type $user
 */
function googleoauth_callback() {
    global $user;
    $client = googleoauth_getclient();
    $oauth2 = new Google_Oauth2Service($client);

    if (isset($_GET['code'])) {
        $client->authenticate($_GET['code']);
        $_SESSION['token'] = $client->getAccessToken();
        $gaccount = $oauth2->userinfo->get();

        if (isset($_SESSION['token'])) {
            $client->setAccessToken($_SESSION['token']);
        }
        if ($client->getAccessToken()) {
            // The access token may have been updated lazily.
            $_SESSION['token'] = $client->getAccessToken();
        }
        // check existing users
        $account = user_load_by_mail($gaccount['email']);
        if (!$account) {
            if (googleoauth_register($client, $gaccount)) {
                $account = user_load_by_mail($gaccount['email']);
            }
        }
        if (isset($_SESSION['token'])) {
            $client->setAccessToken($_SESSION['token']);
        }
        if ($client->getAccessToken()) {
            // The access token may have been updated lazily.
            $_SESSION['token'] = $client->getAccessToken();
            $user = $account;
        }
        drupal_goto('googleoauth-callback/final');
    }
    if (isset($_SESSION['token'])) {
        $client->setAccessToken($_SESSION['token']);
    }
    if ($client->getAccessToken()) {
        $gaccount = $oauth2->userinfo->get();
        // The access token may have been updated lazily.
        $_SESSION['token'] = $client->getAccessToken();
        $user = user_load_by_mail($gaccount['email']);
        drupal_goto('user');
    } else {
        googleoauth_logout($client);
        drupal_set_message(t('Error to authentication'), 'error');
        drupal_goto();
    }
}

/**
 * User registration
 * @param type $client
 * @param type $gaccount
 */
function googleoauth_register($client, $gaccount) {
    // create new account if allowed
    if (variable_get('user_register', 1) || variable_get('googleoauth_allow_registration', 0)) {
        // Check allowed domain names.
        $allowed_domains = variable_get('googleoauth_allowed_domains', '');
        $is_domains_blocked = variable_get('googleoauth_block_domains', '');
        if (!empty($allowed_domains)) {
            $allowed_domains = preg_split('/\r\n|\r|\n/', $allowed_domains);
            
            if (count($allowed_domains)) {
                list($userName, $mailDomain) = explode("@", $gaccount['email']);
                if ($is_domains_blocked) {
                    if ((in_array($mailDomain, $allowed_domains))) {
                        googleoauth_logout($client);
                        drupal_set_message(t('User with email domain @!domain is restrictred to use this site.'
                                        , array('!omain' => $mailDomain)), 'error');
                        drupal_goto();
                    }
                } else {
                    if (!(in_array($mailDomain, $allowed_domains))) {
                        googleoauth_logout($client);
                        drupal_set_message(t('User with email domain @!domain is restrictred to use this site.'
                                        , array('!domain' => $mailDomain)), 'error');
                        drupal_goto();
                    }
                }
            }
        }
        $check = db_query("SELECT COUNT(*) FROM {users} WHERE name = :name", array(':name' => $gaccount['name']))->fetchField();
        if($check) 
            $gaccount['name'] .= '_'.$check;
        $account = new stdClass;
        $account->is_new = TRUE;
        $account->name = $gaccount['name'];
        $account->pass = user_password();
        $account->mail = $gaccount['email'];
        $account->init = $gaccount['email'];
        $account->status = TRUE;
        $account->roles = array(DRUPAL_AUTHENTICATED_RID => TRUE);
        $account->timezone = variable_get('date_default_timezone', '');
        user_save($account);
        return TRUE;
    }
    // 
    else {
        drupal_set_message(t("New Registrations are not allowed"), 'error');
        googleoauth_logout($client);
        drupal_goto();
    }
}