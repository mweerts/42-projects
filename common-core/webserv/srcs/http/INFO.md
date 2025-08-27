LOCATIONS :

SI alias et root en meme temps :


Dans nginx (et la plupart des serveurs web) :
alias a la priorité et root est ignoré
nginx émet généralement un warning dans les logs
Seule la directive alias est prise en compte


alias - Remplacement du chemin
Avec alias, le serveur remplace complètement le chemin de la location.

Exemple avec alias :

location /images {
    alias /var/www/pictures;
}


Résolution :

URL demandée : http://localhost/images/photo.jpg
Chemin final : /var/www/pictures/photo.jpg (le /images est remplacé par /var/www/pictures)


root - Concaténation du chemin
Avec root, le serveur ajoute le chemin de la location au chemin spécifié.

Exemple avec root :

location /images {
    root /var/www;
}


Résolution :

URL demandée : http://localhost/images/photo.jpg
Chemin final : www + /images/photo.jpg = /var/www/images/photo.jpg




