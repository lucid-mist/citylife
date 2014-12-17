#include "GeometryCollection.hpp"

using namespace GIMS_GeometryCollection;



/*append a geometry to the list*/
void append(GIMSGeometry &g){
    size += 1;
    if( size > allocatedSize ){
        this->list = (GIMS_Geometry **)realloc(this->list, this->size * sizeof(GIMS_Geometry *));
        this->allocatedSize = this->size;
    }

    this->list[size-1] = g;
}

/*create a copy of this object*/
GIMS_GeometryCollection *clone () {
    GIMS_GeometryCollection *fresh = new GIMS_GeometryCollection(this->size);
    memcpy(fresh->list, this->list, this->size*sizeOf(GIMSGeometry *));
    return fresh;
}

/*returns a geometry collection that is a subset of this and where all elements intersect the arg box*/
GIMS_Geometry *clipToBox ( GIMS_BoundingBox &box ){
    GIMS_GeometryCollection *clipped = NULL;

    for( int i = 0; i<this->size; i++ ){
        GIMS_Geometry *g = this->list[i]->clipToBox(box);

        if( g != NULL ){
            if( clipped == NULL ){
                clipped = new GIMS_GeometryCollection(1);
            }
            clipped->append(g);
        }
    }
    return clipped;
}

GIMS_GeometryCollection(int size){
    this->type = GEOMETRYCOLLECTION;
    this->size = this->allocatedSize = size;
    this->list = (GIMS_Geometry **)malloc(size, sizeof(GIMS_Geometry *));
}

GIMS_GeometryCollection(){
    this->type = GEOMETRYCOLLECTION;
    this->list = NULL;
    this->size = this->allocatedSize = 0;
}

~GIMS_GeometryCollection(){
    if(this->list != NULL)
        free(this->list);
}
