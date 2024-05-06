#ifndef DATA_HPP
#define DATA_HPP
#include<string>

//batasan tegangan ijin
//deformasi geser tak terkekang
const double stress_allowable_unconfined = 7.0; //MPa 

//deformasi geser terkekang
const double stress_allowable_confined = 7.7; //MPa 

//data beban
constexpr double dead_load = 2400.; // kN Beban Mati
constexpr double live_load = 1200.; // kN Beban Hidup
constexpr double horizontal_deformation = 100.; //mm Gerakan Horizontal Bangunan Atas
constexpr double rotation = 0.015; // rad 
constexpr double beam_width = 750.; // mm Lebar sayap girder (sayap profil balok baja)

//data elastomer
//data fisik elastomer
const std::string hardness = "55 Shore A";
constexpr double modulus_shear = 0.6; //MPa Modulus Geser (G)
constexpr double total_compression_load = 3600.; // kN Total Beban Kompresi (Pt)
constexpr double limit_of_delamination_stress = 7.; // MPa Batas Tegangan Delaminasi

//dimensi elastomer
constexpr double width_w = 725.; //mm lebar elastomer
constexpr double length_l = 740.; //mm panjang elastomer
constexpr double total_rubber_thickness_t = 200.; //mm tebal total karetnya saja (jumlah lapisan karet x tebal lapisan karet + cover atas + cover bawah)
constexpr double rubber_layer_thickness = 16.; //mm tebal lapisan karet
constexpr double cover_layer_thickness = 4.; //mm tebal lapisan penutup

//lapisan elastomer
constexpr double n_layer_interior = 12.; //buah jumlah lapisan karet interior
constexpr double n_layer_exterior = 0.; //buah

//rotasi layan
const double rot_transversal = 0.005; //rad
const double rot_longitudinal = 0.005; //rad

//lapisan pelat baja
constexpr double layer_steel_plate_thickness = 3.0; //mm
constexpr double fy__ = 240.; // MPa  //batas ulur pelata baja
constexpr double delta_fth = 31.; // MPa  //constant amplitude fatique threshold

#endif