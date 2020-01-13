#define SERVER_HEAD           "<!DOCTYPE html><html lang=\"en\"><head><meta charset='utf-8'><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\"/><title>{v}</title>"
#define SERVER_STYLE          "<style> .c{text-align:center;}div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align:center;font-family:verdana;} button{border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .btn-blue{background-color:#1fa3ec;} .btn-red{background-color:#ec2d1f;} .btn-green{background-color:#22c21f;} .q{float:right;width:64px;text-align:right;} .l{background:url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\")no-repeat left center;background-size:1em;} </style>"
#define SERVER_HEAD_END       "</head><body><div style=\"text-align:left;display:inline-block;min-width:260px;\">"

#define SERVER_PORTAL_OPTIONS "<br/><form action=\"/info\" method=\"get\"><button class=\"btn-blue\">Информация</button></form><br/><form action=\"/settings\"method=\"get\"><button class=\"btn-blue\">Настройки</button></form><br/><form action=\"/reboot\"method=\"post\"><button class=\"btn-red\">Перезагрузка</button></form><br/><form action=\"/reset\"method=\"post\"><button class=\"btn-red\">Сброс настроек</button></form>"
#define SERVER_PORTAL_BACK    "<br/><form action=\"/\" method=\"get\"><button class=\"btn-blue\">Назад</button></form>"

#define SERVER_VALUE          "<br/><br/><div class=\"value\"></div>"

#define SERVER_FORM_START     "<form method='get'action='/save'><br/>"
#define SERVER_FORM_PARAM     "<br/><input id='{i}'name='{n}'length={l} placeholder='{p}'value='{v}'>"
#define SERVER_FORM_END       "<br/><br/><button type='submit' class=\"btn-green\">Сохранить</button></form>"

#define SERVER_SAVED          "<div>Настройки сохранены<br/>Перезагрузка...</div>"
#define SERVER_RESET          "<div>Сброс настроек...</div>"
#define SERVER_REBOOT         "<div>Перезагрузка...</div>"

#define SERVER_END            "</div></body></html>"
