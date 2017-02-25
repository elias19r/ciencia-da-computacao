-- sudo apt-get install postgres
-- sudo su - postgres
-- createuser -W msgr_user
-- createdb msgr_db -O msgr_user

CREATE TABLE "users" (
	"id"       SERIAL       NOT NULL PRIMARY KEY,
	"username" VARCHAR(32)  NOT NULL UNIQUE, -- ele cria indice quando e' UNIQUE?
	"password" VARCHAR(40)  NOT NULL,
	"email"    VARCHAR(100) NOT NULL,
	"name"     VARCHAR(100) DEFAULT NULL,
	"country"  VARCHAR(100) DEFAULT NULL
);

CREATE TABLE "groups" (
	"id"      SERIAL      NOT NULL PRIMARY KEY,
	"user_id" INTEGER     NOT NULL REFERENCES "users"(id),
	"name"    VARCHAR(32) NOT NULL
);

CREATE TABLE "groups_users" (
	"group_id" INTEGER NOT NULL REFERENCES "groups"(id),
	"user_id"  INTEGER NOT NULL REFERENCES "users"(id)
);

INSERT INTO "users" ("username", "password", "email", "name", "country") VALUES ('elias', 'QWERTY', 'elias@localhost', 'Elias Rodrigues', 'BR');

-- ALTER USER msgr_user WITH PASSWORD 'QWERTY';

