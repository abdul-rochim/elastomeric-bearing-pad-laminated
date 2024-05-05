#ifndef ELASTOMERIC_BEARING_H
#define ELASTOMERIC_BEARING_H

// #pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<array>
#include<cmath>

//batasan tegangan ijin
//deformasi geser tak terkekang
const double stress_allowable_unconfined = 7.0; //MPa 

//deformasi geser terkekang
const double stress_allowable_confined = 7.7; //MPa 

//rotasi layan
const double rot_transversal = 0.005; //rad
const double rot_longitudinal = 0.005; //rad

//lapisan elastomer
const double n_layer_interior = 12.; //pieces
const double n_layer_exterior = 0.; //pieces

//lapisan pelat baja
constexpr double layer_steel_plate_thickness = 3.0; //mm
constexpr double fy__ = 240.; // MPa  //batas ulur pelata baja
constexpr double delta_fth = 31.; // MPa  //constant amplitude fatique threshold

namespace Elastomeric_Bearing{
    struct Loads
    {
        friend class Calculation;
        Loads(double dl, double ll, double delta_0_, double tetha_, double f_width) : p_deadload{dl}, p_liveload{ll}, delta_0{delta_0_}, tetha{tetha_}, flange_width{f_width}{}
        
        private:
        double p_deadload; //kN      //beban mati
        double p_liveload; //kN      //beban hidup
        double delta_0; //mm  //perpindahan memanjang jembatan
        double tetha; //rad    //rotasi
        double flange_width; //mm // lebar sayap gelagar
    };

    struct MaterialELastomeric
    {
        friend class Calculation;
        MaterialELastomeric(std::string hardness_, double shear_mod_, double pt, double del_stress_lim) : hardness{hardness_}, shear_mod{shear_mod_}, p_tensionload_unfactored{pt}, delamination_stress_limit{del_stress_lim}{}
        
        private:
        std::string hardness;
        double shear_mod; //MPa
        double p_tensionload_unfactored; //kN //total beban tekan unfactored
        double delamination_stress_limit; // MPa //batas tegangan delaminasi
    };

    struct DimensionElastomeric
    {
        friend class Calculation;
        DimensionElastomeric(double w_, double l_, double t_, double layer_thickness_, double h_cover_, double n_layer_, double fy_){
            this->w = w_;
            this->l = l_;
            this->t = t_;
            this->layer_thickness = layer_thickness_;
            this->h_cover = h_cover_;
            this->n_layer = n_layer_;
            this->fy = fy_;
        }
        
        private:
        double w; //mm //lebar bantalan
        double l; //mm //panjang bantalan
        double t; //mm //tebal bantalan
        double layer_thickness; //mm //tebal lapisan
        double h_cover; //mm //tebal lapisan penutup
        double n_layer; //pieces //jumlah lapisan
        double fy; //MPa // mutu baja pelat
    };

    struct Calculation : Loads, MaterialELastomeric, DimensionElastomeric
    {
        //member:
        inline static double area_elastomer = 0.0;
        inline static double stress_s_ = 0.0;
        inline static double stress_l_= 0.0;
        double rotation_long = 0.;
        double rotation_trans = 0.;
        double hrt = 0.;
        double h_total = 0.;

        Calculation(double dl, double ll, double delta_0_, double tetha_, double f_width,
                    std::string hardness_, double shear_mod_, double pt, double del_stress_lim,
                    double w_, double l_, double t_, double layer_thickness_, double h_cover_, double n_layer_, double fy_) :
                    Loads(dl, ll, delta_0_, tetha_, f_width),
                    MaterialELastomeric(hardness_, shear_mod_, pt, del_stress_lim),
                    DimensionElastomeric(w_, l_, t_, layer_thickness_, h_cover_, n_layer_, fy_)
        {
            area_elastomer = w * l;
            stress_s_ = (p_deadload + p_liveload) * 1000. /  this->area_elastomer;
            stress_l_ = p_liveload * 1000. /  this->area_elastomer;
            this->rotation_long = rot_longitudinal;
            this->rotation_trans = rot_transversal;
            this->hrt = layer_thickness * n_layer_interior + 2 * h_cover;
            this->h_total = layer_thickness * n_layer_interior + 2 * h_cover + layer_steel_plate_thickness * (n_layer_interior + 1.);
        }
        
        double area_elastomeric();
        void print_dimensionelastomeric() const;
        double shape_factor();
        void print_shape_factor();
        void stress_allowable();
        void deformation_shear() const;
        void rotation_allowable();
        void stability() const;
        void plate_thickness() const;
        void summary() const;
        void print_data() const;
    };

    void running();

}

#endif

