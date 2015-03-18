#include "DE9IM.hpp"

TODO(leave this initialization to the user since sometimes not all checks are needed)

DE9IM::DE9IM(GIMS_Geometry *query){
    this->query = query;
    matrix = map<long long int, unsigned int>();
}

DE9IM::~DE9IM(){}


int dim(GIMS_Geometry *g){
    if(g->type == POINT || g->type == MULTIPOINT)
        return 0;
    if(g->type == LINESTRING || g->type == MULTILINESTRING)
        return 1;
    if(g->type == POLYGON || g->type == MULTIPOLYGON)
        return 2;
    perror("unkown type of geometry passed to the 9IM model class\n");
    return -1;
}

void DE9IM::setIntersect(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= INTERSECT;
    else
        matrix[id] = INTERSECT;
}

void DE9IM::setII(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= II;
    else
        matrix[id] = II;
}

void DE9IM::setEI(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= EI;
    else
        matrix[id] = EI;
}

void DE9IM::setIE(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= IE;
    else
        matrix[id] = IE;
}

void DE9IM::setEB(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= EB;
    else
        matrix[id] = EB;
}

void DE9IM::setBE(long long int id){
    map<long long int, unsigned int>::iterator it;
    if( (it = matrix.find(id)) != matrix.end() )
        it->second |= BE;
    else
        matrix[id] = BE;
}

list<long> DE9IM::equals(){
    /*II && !IE && !BE && !EI && !EB*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & II &&
            !(mask & IE)&&
            !(mask & BE)&&
            !(mask & EI)&&
            !(mask & EB)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}

/*TODO: in order to implement disjoint geometry reporting, we 
        need access here to all geometries under consideration*/
list<long> DE9IM::disjoint(){
    /*!intersect*/
    return list<long>();
}

list<long> DE9IM::meets(){
    /*!II && intersect*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & INTERSECT &&
            !(mask & II)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}

list<long> DE9IM::contains(){
    /*II && !EI && !EB*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & II &&
            !(mask & EI)&&
            !(mask & EB)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}

list<long> DE9IM::covers(){
    /*intersect && !EI && !EB*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & INTERSECT &&
            !(mask & EI)&&
            !(mask & EB)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}

list<long> DE9IM::intersects(){
    /*II || IB || BI || BB*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if( mask & INTERSECT )
            resultset.push_back( it->first );
    }

    return resultset;
}

list<long> DE9IM::within(){
    /*II && !IE && !BE*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & II &&
            !(mask & IE)&&
            !(mask & BE)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}

list<long> DE9IM::coveredBy(){
    /*intersect && !IE && !BE*/
    list<long> resultset = list<long>();

    for(map<long long int, unsigned int>::iterator it = matrix.begin(); it != matrix.end(); it++){
        unsigned int mask = it->second;
        if(   mask & INTERSECT &&
            !(mask & IE)&&
            !(mask & BE)
        )
            resultset.push_back( it->first );
    }

    return resultset;
}


/*TODO: be able to get the original geometry from the id in order to assess the dimension

list<long> DE9IM::crosses(){
    int dim_a = dim(query), dim_b;

    list<long> resultset = list<long>();
    for(AVLTree::iterator it = II->begin(); it != II->end(); it++){
        dim_b = dim(*it);

        if(dim_a == 0 || dim_b == 0){
            //II
            resultset.push_back((*it)->id);
        
        }else if(dim_a < dim_b &&
                IE->find((*it)->id) != NULL){
            //II && IE
            resultset.push_back((*it)->id);

        }else if(dim_b > dim_a && 
                EI->find((*it)->id) != NULL){
            //II && EI
            resultset.push_back((*it)->id);
        }
    }
    return resultset;
}

list<long> DE9IM::overlaps(){

    int dim_a = dim(query), dim_b;

    list<long> resultset = list<long>();
    for(AVLTree::iterator it = II->begin(); it != II->end(); it++){
        dim_b = dim(*it);

        if( dim_a == dim_b ){
            if(dim_a == 0 || dim_a == 2){
                //II && IE && EI
                if(IE->find((*it)->id) != NULL &&
                   EI->find((*it)->id) != NULL   )
                    resultset.push_back((*it)->id);
            }else{
                //II == 1 && IE && EI
TODO(DE9IM::OVERLAPS | MAJOR! | check if the dimension of the intersection is indeed 1)
                if(IE->find((*it)->id) != NULL &&
                   EI->find((*it)->id) != NULL   )
                    resultset.push_back((*it)->id);
            }
        }
    }
    return resultset;
}
*/