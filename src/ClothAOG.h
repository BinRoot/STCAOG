//
// Created by binroot on 12/4/15.
//

#ifndef STCAOG_CLOTHAOG_H
#define STCAOG_CLOTHAOG_H

#include "STCAOG.h"

class ClothAOG : public STCAOG {
public:
    ClothAOG(std::string filename) : STCAOG(filename) {
        Vertex v_S = add_vertex(STCAOG::SPATIAL, STCAOG::AND, "S");
        Vertex v_T = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "T");
        m_spatial_root_id = v_S.id();
        m_temporal_root_id = v_T.id();

        // Spatial
        Vertex v_table = add_vertex(STCAOG::SPATIAL, STCAOG::AND, "table");
        Vertex v_room = add_vertex(STCAOG::SPATIAL, STCAOG::AND, "room");
        Vertex v_cloth = add_vertex(STCAOG::SPATIAL, STCAOG::AND, "cloth");
        Vertex v_tshirt = add_vertex(STCAOG::SPATIAL, STCAOG::OR, "tshirt");
        add_edge(v_S, v_table);
        add_edge(v_S, v_cloth);
        add_edge(v_S, v_room);
        add_edge(v_room, v_table);
        add_edge(v_room, v_cloth);
        add_edge(v_cloth, v_tshirt);

        // Temporal
        Vertex v_human = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "human");
        Vertex v_robot = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "robot");
        Vertex v_fold1 = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "fold1");
        Vertex v_moveL = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "moveL");
        Vertex v_gripL = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "gripL");
        Vertex v_moveL2 = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "moveL");
        Vertex v_releaseL = add_vertex(STCAOG::TEMPORAL, STCAOG::AND, "releaseL");
        add_edge(v_T, v_human);
        add_edge(v_T, v_robot);
        add_edge(v_human, v_fold1);
        add_edge(v_robot, v_fold1);
        add_edge(v_fold1, v_moveL);
        add_edge(v_fold1, v_gripL);
        add_edge(v_fold1, v_moveL2);
        add_edge(v_fold1, v_releaseL);
    }
private:
};


#endif //STCAOG_CLOTHAOG_H
