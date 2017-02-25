-- sudo apt-get install postgres
-- sudo -u postgres createuser -W messenger
-- sudo -u postgres createdb messengerdb -O messenger

CREATE TABLE "users" (
	"id"       SERIAL       PRIMARY KEY,
	"username" VARCHAR(32)  UNIQUE NOT NULL,
	"password" VARCHAR(16)  NOT NULL,
	"email"    VARCHAR(100) NOT NULL
);


CREATE TABLE "groups" (
	"id"   SERIAL      PRIMARY KEY,
	"name" VARCHAR(80) NOT NULL
);

CREATE TABLE "groups_users" (
	"group_id" INTEGER REFERENCES "groups"(id),
	"user_id"  INTEGER REFERENCES "users"(id)
);

INSERT INTO "users" ("username", "password", "email") VALUES ('elias', 'QWERTY', 'elias@localhost');

ALTER USER messenger WITH PASSWORD 'QWERTY';
