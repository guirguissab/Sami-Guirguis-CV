from controleurs.includes import add_activity

REQUEST_VARS['page_id'] = 'deconnexion'

add_activity(SESSION['HISTORIQUE'], "déconnexion")
SESSION.pop('JOUEUR_COURANT', None)
REQUEST_VARS['message'] = "Vous êtes déconnecté."
REQUEST_VARS['message_class'] = "alert-success"
