//
// Created by binroot on 12/4/15.
//

#ifndef STCAOG_STCAOG_H
#define STCAOG_STCAOG_H

#include <graph/graph.h>

class STCAOG {
public:
    STCAOG(std::string filename);
    enum AO { AND, OR };
    enum STC { SPATIAL, TEMPORAL, CAUSAL, FLUENT };
    struct CausalIdentity {
        VertexIdType vid;
        std::string temporal_name;
        VertexIdType temporal_id;
        std::string fluent_name;
        VertexIdType fluent_id;
    };
    Vertex add_vertex(STC stc, AO ao, std::string name);
    Edge add_edge_unique(const VertexIdType v1_id, const VertexIdType v2_id);
    Edge add_edge(const Vertex& v1, const Vertex& v2);
    Edge add_edge(const VertexIdType v1_id, const VertexIdType v2_id);
    std::string show_dot();
    void write_dot(std::ostream& os);
    void write_dot_file(std::string filename);
    unsigned long size();
    bool find_vertex_by_name(std::string name, VertexIdType& vid);
    bool is_modified();
    std::string get_name(VertexIdType vid);
    STC get_stc(VertexIdType vid);
    std::string get_aog_dir();
    int num_spatial();
    int num_temporal();
    int num_causal();
    int num_fluent();
//    void save();
//    bool load();
    std::vector<VertexIdType> get_fluents();
    bool get_parent_with_prefix(std::string prefix_s, VertexIdType current_vid, VertexIdType& vid);
    std::vector<CausalIdentity> get_out_causals(const VertexIdType& vid);
    std::vector<CausalIdentity> get_causals();
    std::string get_in_temporal_name(VertexIdType vid);
    VertexIdType get_in_temporal_id(VertexIdType vid);
    std::string get_out_fluent_name(VertexIdType vid);
    VertexIdType get_out_fluent_id(VertexIdType vid);
    std::vector<std::shared_ptr<Edge>> get_out_edges(const VertexIdType& vid);
    std::vector<std::shared_ptr<Edge>> get_in_edges(const VertexIdType& vid);
    bool has_unique_ancestor_by_name(VertexIdType vid, std::string name);
    bool get_sibling_by_prefix(VertexIdType child, std::string name, VertexIdType& sibling);
    void set_filename(std::string filename);
protected:
    Graph g;
    VertexIdType m_spatial_root_id;
    VertexIdType m_temporal_root_id;
private:
    const std::string AO_PROP = "AO";
    const std::string STC_PROP = "STC";
    const std::string NAME_PROP = "name";
    const std::string SPATIAL_STR = "S";
    const std::string TEMPORAL_STR = "T";
    const std::string CAUSAL_STR = "C";
    const std::string FLUENT_STR = "F";
    const std::string AND_STR = "A";
    const std::string OR_STR = "O";
    std::string m_aog_dir;
    int m_num_spatial = 0;
    int m_num_temporal = 0;
    int m_num_causal = 0;
    int m_num_fluent = 0;
    bool m_modified = true;
    std::string ROS_PATH = "src/mind_graph/src/";
};

#endif //STCAOG_STCAOG_H
