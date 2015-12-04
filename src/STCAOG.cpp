//
// Created by robot on 7/23/15.
//

#include "STCAOG.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

using namespace std;
//
//bool copy_dir(path const & source, path const & destination) {
//    try
//    {
//        // Check whether the function call is valid
//        if(
//                !exists(source) ||
//                !is_directory(source)
//                )
//        {
//            std::cerr << "Source directory " << source.string()
//            << " does not exist or is not a directory." << '\n'
//                    ;
//            return false;
//        }
//        if(exists(destination))
//        {
//            std::cerr << "Destination directory " << destination.string()
//            << " already exists." << '\n'
//                    ;
//            return false;
//        }
//        // Create the destination directory
//        if(!create_directory(destination))
//        {
//            std::cerr << "Unable to create destination directory"
//            << destination.string() << '\n'
//                    ;
//            return false;
//        }
//    }
//    catch(filesystem_error const & e)
//    {
//        std::cerr << e.what() << '\n';
//        return false;
//    }
//    // Iterate through the source directory
//    for(
//            directory_iterator file(source);
//            file != directory_iterator(); ++file
//            )
//    {
//        try
//        {
//            path current(file->path());
//            if(is_directory(current))
//            {
//                // Found directory: Recursion
//                if(
//                        !copy_dir(
//                                current,
//                                destination / current.filename()
//                        )
//                        )
//                {
//                    return false;
//                }
//            }
//            else
//            {
//                // Found file: Copy
//                copy_file(
//                        current,
//                        destination / current.filename()
//                );
//            }
//        }
//        catch(filesystem_error const & e)
//        {
//            std:: cerr << e.what() << '\n';
//        }
//    }
//    return true;
//}

struct STCAOGNode {
    string stc;
    string label;
    string ao;
    string name;
    string image;
    string color;
    string style;
    string shape;
    VertexIdType node_id;
};

struct STCAOGEdge {

};

STCAOG::STCAOG(std::string filename) {
    m_aog_dir = filename;
}

std::string STCAOG::get_aog_dir() {
    return m_aog_dir;
}

std::string STCAOG::get_out_fluent_name(VertexIdType vid) {
    for (auto& edge : get_out_edges(vid)) {
        VertexIdType tgt = edge->target();
        if (get_stc(tgt) == FLUENT) {
            return get_name(tgt);
        }
    }
    return "";
}

VertexIdType STCAOG::get_out_fluent_id(VertexIdType vid) {
    for (auto& edge : get_out_edges(vid)) {
        VertexIdType tgt = edge->target();
        if (get_stc(tgt) == FLUENT) {
            return tgt;
        }
    }
    return 0;
}

std::string STCAOG::get_in_temporal_name(VertexIdType vid) {
    for (auto& edge : get_in_edges(vid)) {
        VertexIdType src = edge->source();
        if (get_stc(src) == TEMPORAL) {
            return get_name(src);
        }
    }
    return "";
}

VertexIdType STCAOG::get_in_temporal_id(VertexIdType vid) {
    for (auto& edge : get_in_edges(vid)) {
        VertexIdType src = edge->source();
        if (get_stc(src) == TEMPORAL) {
            return src;
        }
    }
    return 0;
}

std::vector<STCAOG::CausalIdentity> STCAOG::get_causals() {
    vector<STCAOG::CausalIdentity> ids;
    vector<VertexIdType> causals = g.find_vertices_by_property(STC_PROP, CAUSAL_STR);
    for (auto& vid : causals) {
        STCAOG::CausalIdentity ci;
        ci.vid = vid;
        ci.fluent_name = get_out_fluent_name(vid);
        ci.fluent_id = get_out_fluent_id(vid);
        ci.temporal_name = get_in_temporal_name(vid);
        ci.temporal_id = get_in_temporal_id(vid);
        ids.push_back(ci);
    }
    return ids;
}

std::vector<STCAOG::CausalIdentity> STCAOG::get_out_causals(const VertexIdType& vid) {
    vector<STCAOG::CausalIdentity> ids;
    for (auto& edge : get_out_edges(vid)) {
        VertexIdType tgt = edge->target();
        if (get_stc(tgt) == CAUSAL) {
            STCAOG::CausalIdentity ci;
            ci.vid = tgt;
            ci.fluent_name = get_out_fluent_name(tgt);
            ci.fluent_id = get_out_fluent_id(tgt);
            ci.temporal_name = get_in_temporal_name(tgt);
            ci.temporal_id = get_in_temporal_id(tgt);
            ids.push_back(ci);
        }
    }
    return ids;
}

std::vector<std::shared_ptr<Edge>> STCAOG::get_out_edges(const VertexIdType& vid) {
    return g.get_out_edges(vid);
}

std::vector<std::shared_ptr<Edge>> STCAOG::get_in_edges(const VertexIdType& vid) {
    return g.get_in_edges(vid);
}

void STCAOG::set_filename(std::string filename) {
    m_aog_dir = filename;
}

vector<VertexIdType> STCAOG::get_fluents() {
    return g.find_vertices_by_property(STC_PROP, FLUENT_STR);
}

bool STCAOG::get_parent_with_prefix(string prefix_s, VertexIdType current_vid, VertexIdType& vid) {
    for (auto& e : get_in_edges(current_vid)) {
        string parent_name = get_name(e->source());
        if (!parent_name.compare(0, prefix_s.size(), prefix_s)) {
            vid = e->source();
            return true;
        }
    }
    return false;
}


//bool STCAOG::load() {
//
//    // create empty icons folder
//    if (is_directory(ROS_PATH + "icons/")) {
//        boost::filesystem::remove_all(ROS_PATH + "icons/");
//    }
//
//    if (!is_directory(m_aog_dir)) {
//        create_directory(m_aog_dir);
//        boost::filesystem::create_directory(ROS_PATH + "icons/");
//        return false;
//    } else {
//        // copy the icons folder over
//        cout << "copying icons from " << m_aog_dir + "icons/" << endl;
//        cout << "pasting icons to " << ROS_PATH + "icons/" << endl;
//        copy_dir(m_aog_dir + "icons/", ROS_PATH + "icons/");
//        cout << "done copying" << endl;
//
//
//        typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, STCAOGNode, STCAOGEdge> graph_t;
//        typedef boost::graph_traits <graph_t >::out_edge_iterator out_edge_iterator;
//
//        graph_t graph;
//
//        boost::dynamic_properties dp;
//        dp.property("node_id", boost::get(&STCAOGNode::node_id, graph));
//        dp.property("name", boost::get(&STCAOGNode::name, graph));
//        dp.property("label", boost::get(&STCAOGNode::label, graph));
//        dp.property(AO_PROP, boost::get(&STCAOGNode::ao, graph));
//        dp.property(STC_PROP, boost::get(&STCAOGNode::stc, graph));
//        dp.property("image", boost::get(&STCAOGNode::image, graph));
//        dp.property("color", boost::get(&STCAOGNode::color, graph));
//        dp.property("style", boost::get(&STCAOGNode::style, graph));
//        dp.property("shape", boost::get(&STCAOGNode::shape, graph));
//
//        std::string filename = m_aog_dir + "aog.dot";
//        std::ifstream fin(filename.c_str());
//        std::string str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
//
//        boost::read_graphviz(str, graph, dp, "node_id");
//
////        std::cout << "There are " << boost::num_vertices(graph) << " vertices." << std::endl;
//
//        unordered_map<VertexIdType, VertexIdType> v_seen;
//
//        auto vs = boost::vertices(graph);
//
//        for (auto vit = vs.first; vit != vs.second; ++vit) {
//            STCAOGNode v_STCAOGNode = graph[*vit];
//            STC v_stc;
//            if (v_STCAOGNode.stc.compare(SPATIAL_STR) == 0) v_stc = SPATIAL;
//            else if (v_STCAOGNode.stc.compare(TEMPORAL_STR) == 0) v_stc = TEMPORAL;
//            else if (v_STCAOGNode.stc.compare(CAUSAL_STR) == 0) v_stc = CAUSAL;
//            else v_stc = FLUENT;
//            AO v_ao;
//            string v_ao_str;
//            if (v_STCAOGNode.ao.compare(AND_STR) == 0) {
//                v_ao = AND;
//                v_ao_str = AND_STR;
//            } else {
//                v_ao = OR;
//                v_ao_str = OR_STR;
//            }
//            Vertex v = add_vertex(v_stc, v_ao, v_STCAOGNode.name);
//            v_seen[v_STCAOGNode.node_id] = v.id();
//
//            // check if AOG_DIR/<vid> exists
//            if (is_directory(m_aog_dir + to_string(v_STCAOGNode.node_id))) {
//                vector<Mat> data;
//                for (int i = 0; i >= 0; i++) {
//                    string data_filepath = m_aog_dir + to_string(v_STCAOGNode.node_id) + "/" + to_string(i) + ".yml";
//                    if (is_regular_file(data_filepath)) {
////                        Mat datum = cv::imread(data_filepath, CV_LOAD_IMAGE_UNCHANGED);
//                        Mat datum;
//                        cv::FileStorage storage(data_filepath, cv::FileStorage::READ);
//                        storage["data"] >> datum;
//                        storage.release();
//                        cout << "vertex " << v_STCAOGNode.node_id << " " << v_STCAOGNode.name <<  ": loaded file " << i << ": " << datum.size() << " " << datum.type() << endl;
//                        data.push_back(datum);
//                    } else break;
//                }
//                if (!data.empty()) {
//                    property_blob[v_seen[v_STCAOGNode.node_id]] = vector<Mat>(data.size());
//                    for (int i = 0; i < data.size(); i++) {
//                        property_blob[v_seen[v_STCAOGNode.node_id]][i] = data[i].clone();
//                    }
//                }
//            }
//        }
//
//        boost::graph_traits<graph_t>::edge_iterator ei, ei_end;
//        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
//            VertexIdType v_src = graph[source(*ei, graph)].node_id;
//            VertexIdType v_tgt = graph[target(*ei, graph)].node_id;
//            add_edge(v_seen[v_src], v_seen[v_tgt]);
////            cout << (*g.get_vertex(v_seen[v_src]))[NAME_PROP] <<
////                    " " <<
////                    (*g.get_vertex(v_seen[v_src]))[AO_PROP] <<
////                    " --> " <<
////                    (*g.get_vertex(v_seen[v_tgt]))[NAME_PROP] <<
////                    " " <<
////                    (*g.get_vertex(v_seen[v_tgt]))[AO_PROP] <<
////                    endl;
//        }
//
//        return true;
//    }
//}
//
//void STCAOG::save() {
//    if (!is_directory(m_aog_dir)) {
//        create_directory(m_aog_dir);
//    } else {
//        remove_all(m_aog_dir);
//        create_directory(m_aog_dir);
//    }
//    for (const auto &it : property_blob) {
//        VertexIdType vid = it.first;
//        vector<Mat> data = it.second;
//        string v_dir = m_aog_dir + to_string(vid) + "/";
//        create_directory(v_dir);
//        for (int i = 0; i < data.size(); i++) {
////            imwrite(v_dir + to_string(i) + ".bmp", data[i]);
//            cv::FileStorage storage(v_dir + to_string(i) + ".yml", cv::FileStorage::WRITE);
//            storage << "data" << data[i];
//            storage.release();
//        }
//    }
//    if (is_directory(ROS_PATH + "icons")) {
//        copy_dir(ROS_PATH + "icons", m_aog_dir + "icons/");
//    }
//    write_dot_file(m_aog_dir + "aog.dot");
//}

Vertex STCAOG::add_vertex(STCAOG::STC stc, STCAOG::AO ao, std::string name) {
    VertexIdType vid = g.get_unused_id();
    Vertex v(vid);
    switch (ao) {
        case AND:
            v[AO_PROP] = make_value<string>(AND_STR);
            break;
        case OR:
            v[AO_PROP] = make_value<string>(OR_STR);
            break;
        default:
            v[AO_PROP] = make_value<string>("x");
    }
    switch (stc) {
        case SPATIAL:
            m_num_spatial++;
            v[STC_PROP] = make_value<string>(SPATIAL_STR);
            break;
        case TEMPORAL:
            m_num_temporal++;
            v[STC_PROP] = make_value<string>(TEMPORAL_STR);
            break;
        case CAUSAL:
            m_num_causal++;
            v[STC_PROP] = make_value<string>(CAUSAL_STR);
            break;
        case FLUENT:
            m_num_fluent++;
            v[STC_PROP] = make_value<string>(FLUENT_STR);
            break;
        default:
            v[STC_PROP] = make_value<string>("x");
    }
    v[NAME_PROP] = make_value<string>(name);
    g.add_vertex(v);
    m_modified = true;
    return v;
}

int STCAOG::num_spatial() {
    return m_num_spatial;
}

int STCAOG::num_temporal() {
    return m_num_temporal;
}

int STCAOG::num_causal() {
    return m_num_causal;
}

int STCAOG::num_fluent() {
    return m_num_fluent;
}

Edge STCAOG::add_edge_unique(const VertexIdType v1_id, const VertexIdType v2_id) {
    for (auto& edge : g.get_out_edges(v1_id)) {
        if (edge->target() == v2_id) {
            return *edge;
        }
    }
    Edge e(v1_id, v2_id);
    g.add_edge(e);
    m_modified = true;
    return e;
}

Edge STCAOG::add_edge(const Vertex &v1, const Vertex &v2) {
    Edge e(v1.id(), v2.id());
    g.add_edge(e);
    m_modified = true;
    return e;
}

Edge STCAOG::add_edge(const VertexIdType v1_id, const VertexIdType v2_id) {
    Edge e(v1_id, v2_id);
    g.add_edge(e);
    m_modified = true;
    return e;
}

void STCAOG::write_dot(std::ostream& os) {
    g.write_graphviz(os, [&](const std::shared_ptr<Vertex>& v) -> std::string {
                         std::stringstream ss;
                         std::string name = get_value<std::string>((*v)[STCAOG::NAME_PROP]);

                         std::string prefix_f = "f_";
                         std::string prefix_c = "c_";
                         if (name.compare(0, prefix_f.size(), prefix_f) &&
                             name.compare(0, prefix_c.size(), prefix_c)) {
                             ss << name;
                         }

                         ss << "\", name=\"" << name;

                         ss << "\", shape=\"";
                         std::string stc_prop = get_value<string>((*v)[STCAOG::STC_PROP]);
                         if (stc_prop.compare(STCAOG::SPATIAL_STR) == 0) {
                             ss << "oval";
                             ss << "\", color=\"";
                             ss << "#8888ff";
                             ss << "\", " << STC_PROP << "=\"";
                             ss << SPATIAL_STR;
                         } else if (stc_prop.compare(STCAOG::TEMPORAL_STR) == 0) {
                             ss << "diamond";
                             ss << "\", color=\"";
                             ss << "#11d330";
                             ss << "\", " << STC_PROP << "=\"";
                             ss << TEMPORAL_STR;
                         } else if (stc_prop.compare(STCAOG::CAUSAL_STR) == 0) {
                             ss << "box";
                             ss << "\", color=\"";
                             ss << "yellow";
                             ss << "\", " << STC_PROP << "=\"";
                             ss << CAUSAL_STR;
                         } else if (stc_prop.compare(STCAOG::FLUENT_STR) == 0) {
                             ss << "egg";
                             ss << "\", color=\"";
                             ss << "#000000";
                             ss << "\", " << STC_PROP << "=\"";
                             ss << FLUENT_STR;
                         } else {
                             ss << "hexagon";
                         }
                         ss << "\", style=\"";
                         std::string ao_prop = get_value<string>((*v)[STCAOG::AO_PROP]);
                         if (ao_prop.compare(STCAOG::AND_STR) == 0) {
                             ss << "filled";
                             ss << "\", " << AO_PROP << "=\"";
                             ss << AND_STR;
                         } else if (ao_prop.compare(STCAOG::OR_STR) == 0) {
                             ss << "bold";
                             ss << "\", " << AO_PROP << "=\"";
                             ss << OR_STR;
                         } else {
                             ss << "";
                         }
                         std::string prefix_s = "s_";
                         if (!name.compare(0, prefix_s.size(), prefix_s)) {
                             ss << "\", image=\"";
                             ss << ROS_PATH << "icons/" << name << "_icon.png";
                         }

                         if (!name.compare(0, prefix_f.size(), prefix_f)) {
                             ss << "\", image=\"";
                             ss << ROS_PATH << "icons/" << name << "_icon.png";
                         }

                         if (!name.compare(0, prefix_c.size(), prefix_c)) {
                             ss << "\", image=\"";
                             ss << ROS_PATH << "icons/" << name << "_icon.png";
                         }

//                         std::string prefix_f = "f_";
//                         if (!name.compare(0, prefix_f.size(), prefix_f)) {
//                             ss << "\", image=\"";
//                             ss << "icons/" << name << "_icon.png";
//                         }
                         return ss.str();
                     },
                     nullptr);
}

std::string STCAOG::show_dot() {
    stringstream ss;
    write_dot(ss);
    return ss.str();
}

void STCAOG::write_dot_file(string filename) {
    ofstream myfile;
    myfile.open(filename.c_str());
    write_dot(myfile);
    myfile.close();
}

bool STCAOG::is_modified() {
    bool modified_original = m_modified;
    m_modified = false;
    return modified_original;
}

std::string STCAOG::get_name(VertexIdType vid) {
    return get_value<string>((*g.get_vertex(vid))[STCAOG::NAME_PROP]);
}

STCAOG::STC STCAOG::get_stc(VertexIdType vid) {
    std::string stc = get_value<string>((*g.get_vertex(vid))[STCAOG::STC_PROP]);
    if (stc.compare(SPATIAL_STR) == 0) return SPATIAL;
    else if (stc.compare(TEMPORAL_STR) == 0) return TEMPORAL;
    else if (stc.compare(CAUSAL_STR) == 0) return CAUSAL;
    else return FLUENT;
}

unsigned long STCAOG::size() {
    return g.get_num_vertices();
}

bool STCAOG::find_vertex_by_name(std::string name, VertexIdType& vid) {
    return g.find_vertex_by_property(STCAOG::NAME_PROP, name, vid);
}

// TODO: untested!
bool STCAOG::has_unique_ancestor_by_name(VertexIdType vid, string name) {
    stack<VertexIdType> vid_stack;
    vid_stack.push(vid);
    while (!vid_stack.empty()) {
        VertexIdType current_vid = vid_stack.top();
        vid_stack.pop();
        vector<shared_ptr<Edge>> edges = g.get_in_edges(current_vid);
        if (edges.size() == 1) {
            VertexIdType source = edges[0]->source();
            if (get_name(source).compare(name) == 0) {
                return true;
            } else {
                vid_stack.push(source);
            }
        }
    }
    return false;
}

// TODO: untested!
bool STCAOG::get_sibling_by_prefix(VertexIdType child, std::string prefix, VertexIdType& sibling) {
    for (auto& e : g.get_in_edges(child)) {
        VertexIdType parent_id = e->source();
        for (auto& sib_edge : g.get_out_edges(parent_id)) {
            VertexIdType sibling_id = sib_edge->target();
            if (sibling_id != child) {
                if (!get_name(sibling_id).compare(0, prefix.size(), prefix)) {
                    sibling = sibling_id;
                    return true;
                }
            }
        }
    }
    return false;
}