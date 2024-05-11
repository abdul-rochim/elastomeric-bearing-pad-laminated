#ifndef DATA_HPP
#define DATA_HPP
#include<string>

//batasan tegangan ijin
//deformasi geser tak terkekang
const double stress_allowable_unconfined = 7.0; //MPa 

//deformasi geser terkekang
const double stress_allowable_confined = 7.7; //MPa 

//data beban
constexpr double dead_load = 50.; // kN Beban Mati
constexpr double live_load = 40.; // kN Beban Hidup
constexpr double force_trans_ = 35.; // kN Gaya Horzontal
// constexpr double horizontal_trans = 0.012; //mm Gerakan translasi pada joint reaksi
constexpr double horizontal_deformation = 15.; //mm Gerakan Horizontal Bangunan Atas
constexpr double rotation = 0.015; // rad 
constexpr double beam_width = 200.; // mm Lebar sayap girder (sayap profil balok baja)
// constexpr double coef_friction = 0.2; // koefisien antara beton dan baja

//data elastomer
//data fisik elastomer
const std::string hardness = "50 +-5";
constexpr double modulus_shear = 0.6; //MPa Modulus Geser (G)
constexpr double total_compression_load = 90.; // kN Total Beban Kompresi (Pt)
constexpr double limit_of_delamination_stress = 7.; // MPa Batas Tegangan Delaminasi

//dimensi elastomer
constexpr double width_w = 175.; //mm lebar elastomer
constexpr double length_l = 200.; //mm panjang elastomer
constexpr double total_rubber_thickness_t = 9.*3. + 6.*2 ; //mm tebal total karetnya saja (jumlah lapisan karet x tebal lapisan karet + cover atas + cover bawah)
constexpr double rubber_layer_thickness = 9.; //mm tebal lapisan karet
constexpr double cover_layer_thickness = 6.; //mm tebal lapisan penutup

//lapisan elastomer
constexpr double n_layer_interior = 3.; //buah jumlah lapisan karet interior
constexpr double n_layer_exterior = 0.; //buah

//rotasi layan
const double rot_transversal = 0.005; //rad
const double rot_longitudinal = 0.005; //rad

//lapisan pelat baja
constexpr double layer_steel_plate_thickness = 2.0; //mm
constexpr double fy__ = 240.; // MPa  //batas ulur pelata baja
constexpr double delta_fth = 31.; // MPa  //constant amplitude fatique threshold

//anchor bolt
constexpr double dia_bolt = 22.; // mm
constexpr double fy_bolt = 500.; //MPa
constexpr double n_bolt = 4.0; // buah jumlah bolt

#endif