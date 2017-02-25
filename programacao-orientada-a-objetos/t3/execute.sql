CREATE TABLE "users" (
	"id"       SERIAL       PRIMARY KEY,
	"username" VARCHAR(32)  UNIQUE NOT NULL, -- INDEX ??
	"password" VARCHAR(16)  NOT NULL,
	"email"    VARCHAR(100) NOT NULL
);

