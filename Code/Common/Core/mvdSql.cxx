namespace
{

/****************************************************************************/
//
// 'SQL_DB_CREATE' Generated from file 'sql_db_create.sql'
const char* SQL_DB_CREATE[] = {
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        hash  TEXT NOT NULL,\n"
"        alias TEXT\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_dataset_hash ON dataset( hash )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE tag\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        label TEXT NOT NULL\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_tag_label ON tag( label )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE tag_node\n"
"(\n"
"        id        INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        parent_id INTEGER,\n"
"        tag_id    INTEGER,\n"
"        level     INTEGER,\n"
"        path      TEXT,\n"
"        FOREIGN KEY( parent_id ) REFERENCES tag_node( id ),\n"
"        FOREIGN KEY( tag_id )  REFERENCES tag( id )\n"
")\n"
";",
"CREATE INDEX idx_tag_node_tid ON tag_node( tag_id )\n"
";",
"CREATE INDEX idx_tag_node_pid ON tag_node( parent_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_membership(\n"
"       dataset_id       INTEGER,\n"
"       tag_id           INTEGER,\n"
"       PRIMARY KEY( dataset_id, tag_id ),\n"
"       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,\n"
"       FOREIGN KEY( tag_id ) REFERENCES tag( id ) ON DELETE CASCADE\n"
")\n"
";",
"CREATE INDEX idx_ds_membership_dataset_id ON dataset_membership( dataset_id )\n"
";",
"CREATE INDEX idx_ds_membership_dataset_tag_id ON dataset_membership( tag_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_attribute\n"
"(\n"
"        id         INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        rank       INTEGER NOT NULL,\n"
"        name       TEXT NOT NULL,\n"
"        value      TEXT\n"
")\n"
";",
"CREATE INDEX idx_dataset_attribute_dsid ON dataset_attribute( dataset_id )\n"
";",
};

const int SQL_DB_CREATE_COUNT = 12;

/****************************************************************************/
//
// 'SQL_DB_SETUP' Generated from file 'sql_db_setup.sql'
const char* SQL_DB_SETUP[] = {
"-----------------------------------------------------------------------------\n"
"INSERT INTO tag( id, label ) VALUES( 1, 'Root' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 2, 'Datasets' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 3, 'Cached' )\n"
";",
"-----------------------------------------------------------------------------\n"
"-- Root\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 1, NULL, 1, 0, '/')\n"
";",
"-- Root/Datasets\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 2, 1,    2, 1, '/1' )\n"
";",
"-- Root/Datasets/Cached\n"
"INSERT INTO tag_node( id, parent_id, tag_id, level, path )\n"
"VALUES( 3, 2,    3, 2, '/1/2' )\n"
";",
};

const int SQL_DB_SETUP_COUNT = 6;

/****************************************************************************/
//
// 'SQL_QUERIES_INSERT' Generated from file 'sql_queries_insert.sql'
const char* SQL_QUERIES_INSERT[] = {
"-----------------------------------------------------------------------------\n"
"-- SQLQ_INSERT_TAG_NODE\n"
"-- Insert tag-node for tag :child_label under parent-node of tag-node\n"
"--  :parent_label.\n"
"INSERT INTO tag_node( parent_id, tag_id, level, path )\n"
"        SELECT -- ID (automatic)\n"
"               tag_node.id AS parent_id,\n"
"               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,\n"
"               tag_node.level+1 AS level,\n"
"               rtrim( tag_node.path, '/' ) || '/' || tag_node.id AS path\n"
"        FROM   tag_node\n"
"        JOIN   tag ON tag_node.tag_id=tag.id\n"
"        WHERE  tag.id=(SELECT tag.id FROM tag WHERE tag.label=:parent_label)\n"
";",
"-----------------------------------------------------------------------------\n"
"-- SQLQ_INSERT_DATASET_MEMBERSHIP\n"
"-- Add dataset-membership of dataset %1 to each tag related to\n"
"-- %2 tag-node path list of the form (<id_0>, ...).\n"
"INSERT INTO dataset_membership( dataset_id, tag_id )\n"
"       SELECT %1 AS 'dataset_id',\n"
"              tag_node.tag_id\n"
"       FROM   tag_node\n"
"       JOIN   tag ON tag_node.tag_id=tag.id\n"
"       WHERE  tag_node.id IN (%2)\n"
";",
};

const int SQL_QUERIES_INSERT_COUNT = 2;

/****************************************************************************/
//
// 'SQL_QUERIES_SELECT' Generated from file 'sql_queries_select.sql'
const char* SQL_QUERIES_SELECT[] = {
"-----------------------------------------------------------------------------\n"
"-- SQLQ_SELECT_TAG_NODE_BY_TAG_LABEL\n"
"-- Find tag-node given tag-label.\n"
"SELECT tag_node.id,\n"
"       tag_node.parent_id,\n"
"       tag_node.tag_id,\n"
"       tag_node.level,\n"
"       tag_node.path,\n"
"       tag.label\n"
"FROM tag_node\n"
"JOIN tag ON tag_node.tag_id=tag.id\n"
"WHERE tag_node.tag_id=(SELECT tag.id FROM tag WHERE tag.label=:label)\n"
";",
};

const int SQL_QUERIES_SELECT_COUNT = 1;
} // End of anonymous namespace.