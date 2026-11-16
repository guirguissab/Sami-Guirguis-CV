from model.model_pg import search_human_players_by_pseudo, get_human_player_by_id
from controleurs.includes import add_activity

REQUEST_VARS['page_id'] = 'connexion'

add_activity(SESSION['HISTORIQUE'], "consultation de la page de connexion")

REQUEST_VARS['result'] = []

if POST and 'bouton_rechercher' in POST:
    term = POST['pseudo_recherche'][0].strip()
    result = search_human_players_by_pseudo(SESSION['CONNEXION'], term)
    if not result:
        REQUEST_VARS['message'] = f"Aucun pseudo trouvé pour '{term}'."
        REQUEST_VARS['message_class'] = "alert-warning"
    else:
        REQUEST_VARS['message_info'] = f"{len(result)} pseudo(s) trouvé(s) pour '{term}'."
        REQUEST_VARS['result'] = result
        REQUEST_VARS['message_class'] = "alert-success"

if POST and 'bouton_connexion' in POST:
    id_joueur = POST.get('id_joueur_selectionne', [''])[0]
    if not id_joueur:
        REQUEST_VARS['message'] = "Sélectionnez un pseudo avant de continuer."
        REQUEST_VARS['message_class'] = "alert-warning"
    else:
        joueur = get_human_player_by_id(SESSION['CONNEXION'], id_joueur)
        if joueur is None:
            REQUEST_VARS['message'] = "Le joueur sélectionné n'existe plus."
            REQUEST_VARS['message_class'] = "alert-error"
        else:
            SESSION['JOUEUR_COURANT'] = joueur
            REQUEST_VARS['redirect_accueil'] = True
