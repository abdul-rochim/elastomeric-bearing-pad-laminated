#include "elastomeric_bearing.h"

double Elastomeric_Bearing::Calculation::area_elastomeric(){
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    double area_req = p_compressionload_unfactored * 1000. / delamination_stress_limit;
    myfile << "Perhitungan:\n";
    myfile << "1. Luas Area Elastomer yang diperlukan:\n";
    myfile << "   Syarat lebar(W) < Lebar saya girder\n";
    if (w < flange_width)
        myfile << "   lebar elastomer (W) = " << w << " mm < " << flange_width << " mm  [ OK ]\n";
    else
        myfile << "   lebar elastomer (W) = " << w << " mm > " << flange_width << " mm  [ NOT OK ]\n";
    myfile << "   Aperlu > (Pt x 1000 / batas tegangan delaminasi = " << area_req << " mm2\n";
    double area_used = area_elastomer;
    auto area_lambda = [area_used, area_req]() {
        if (area_used >= area_req)
            return " [OK]";
        else
            return " [Not OK]";
    };
    myfile << "   Luas Elastomer = " << area_used << " mm2  > " << area_req << " mm2 " << area_lambda() << "\n";
    myfile << "   minimal panjang elastomer (L) yang disyaratkan : " << area_req/ w << " mm\n";
    myfile << "   digunakan panjang elastomer (L) : " << l << " mm\n";
    if (l < area_req/ w)
        myfile << "   panjang elastomer (L) = " << l << " mm > " << area_req/ w << " mm  [ OK ]\n";
    else
        myfile << "   panjang elastomer (L) = " << l << " mm < " << area_req/ w << " mm  [ NOT OK ]\n";
    myfile << "   Aperlu > (Pt x 1000 / batas tegangan delaminasi = " << area_req << " mm2\n";
    return area_used;
}

void Elastomeric_Bearing::Calculation::print_dimensionelastomeric() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "2. Asumsi Dimensi - dimensi dalam perletakan elastomer berdasarkan perhitungan Luas diatas:\n";
    myfile << "   Lebar(W)                                 : " << w << " mm\n";
    myfile << "   Panjang(L)                               : " << l << " mm\n";
    myfile << "   Tebal                                    : " << t << " mm\n";
    myfile << "   Tebal Lapisan (hri)                      : " << layer_thickness << " mm\n";
    myfile << "   Tebal Lapisan penutup (h cover)          : " << h_cover << " mm\n";
    myfile << "   Jumlah Lapisan (n)                       : " << n_layer << " buah\n";
    myfile << "   Fy pelat baja                            : " << fy << " mm\n\n";
    myfile.close();
}

double Elastomeric_Bearing::Calculation::shape_factor(){
    double S = w * l / (2. * (w + l) * layer_thickness);
    return S;
}

void Elastomeric_Bearing::Calculation::print_shape_factor(){
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "3. Hitung Shape Factor (Faktor Bentuk) :\n";
    myfile << "   S     = A / (Ip x hri)\n";
    myfile << "   Ip    = 2 x (L+W)\n";
    double S = Calculation::shape_factor();
    myfile << "   S     = " << S << '\n';
    auto sf_lambda = [=] () {
        if(S <= 12 && S > 4)
            return "[ OK ]";
        else
            return "[ NOT OK ]";
    };
    myfile << "      4  < " << S << " <= 12 " << sf_lambda() << "\n";
    myfile.close();
}

void Elastomeric_Bearing::Calculation::stress_allowable(){
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "4. Cek Tegangan Ijin:\n";
    myfile << "   syarat modulus geser: 0.6 MPa <=  G  <=  1.3 MPa\n";
    if(shear_mod >= 0.6 && shear_mod <= 1.3)
        myfile << "   G = " << shear_mod << " MPa, sehingga syarat 0.6 MPa <=  G  <= 1.3 MPa  [ OK ]\n\n";
    else
        myfile << "   G = " << shear_mod << " MPa, sehingga syarat 0.6 MPa <=  G  <=  1.3 MPa  [ NOT OK ]\n";

    double stress_s = stress_s_;
    myfile << "   Stress_s  = (Pdl + Pll) / A   = " << stress_s << " MPa\n";
    double stress_l = stress_l_;
    myfile << "   Stress_l  = Pll/ A            = " << stress_l << " MPa\n";

    auto sl_lambda = [stress_s](auto req_1, auto req_2, auto req_3, auto req_4)-> std::array<std::string,4>{
        std::string ret1 = {" NOT OK "};
        std::string ret2 = {" NOT OK "};
        std::string ret3 = {" NOT OK "};
        std::string ret4 = {" NOT OK "};
        if (stress_s <= req_1)
            ret1 = "[ OK ]";
        if (stress_s <=  req_2)
            ret2 = "[ OK ]";
        if (stress_s <= req_3)
            ret3 = "[ OK ]";
        if (stress_s <= req_4)
            ret4 = "[ OK ]";
        return {ret1, ret2, ret3, ret4};
    };

    //call lambda untuk cek tegangan ijin
    auto data = sl_lambda(stress_allowable_unconfined, 1.0 * shear_mod * Calculation::shape_factor(), stress_allowable_confined, 1.1 * shear_mod * Calculation::shape_factor());
    
    myfile << "   Bantalan dengan deformasi geser yang tidak dikekang\n";
    myfile << "   syarat stress_s <= " << stress_allowable_unconfined << " MPa, sehingga " << stress_s << " MPa <= " << stress_allowable_unconfined << " MPa  " << data.at(0) << "\n";
    myfile << "   syarat stress_s <= 1.0 G S, sehingga " << stress_s << " MPa <= " << 1.0 * shear_mod * Calculation::shape_factor() << " MPa  " << data.at(1) << "\n\n";
    
    myfile << "   Bantalan dengan deformasi geser yang dikekang\n";
    myfile << "   syarat stress_s <= " << stress_allowable_confined <<   " MPa, sehingga " << stress_s << " MPa <= " << stress_allowable_confined << " MPa  " << data.at(2) << "\n";
    myfile << "   syarat stress_s <= 1.1 G S, sehingga " << stress_s << " MPa <= " << 1.1 * shear_mod * Calculation::shape_factor() << " MPa  " << data.at(3) << "\n\n";
    
    myfile.close();
}

void Elastomeric_Bearing::Calculation::deformation_shear() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "5. Cek Deformasi Geser:\n";
    myfile << "   Total Deformasi Geser Rencana, delta_0 = " << delta_0 << " mm\n";
    myfile << "   Deformasi ijin = 2 x delta_0 = " << 2 * delta_0 << " mm\n\n";
    myfile << "   Ketebalan total elastomer (hrt) = (jumlah tebal lapisan internal + jumlah tebal cover)\n";
    myfile << "   hrt = " << hrt << " mm\n";
    if (hrt >= 2 * delta_0)
        myfile << "   hrt >= 2 x delta_0,  " << hrt << " mm >= " << 2 * delta_0 << " mm  [ OK ]\n\n";
    else
        myfile << "   hrt < 2 x delta_0,  " << hrt << " mm < " << 2 * delta_0 << " mm  [ NOT OK ]\n\n";
    if(h_cover < 0.7 * layer_thickness){
        myfile << "   syarat h_cover < 0.7 hri\n";
        myfile << "   " << h_cover << " mm < " << 0.7 * layer_thickness << " mm  [ OK ]\n\n";
    }
    else{
        myfile << "   syarat h_cover < 0.7 hri\n";
        myfile << "   " << h_cover << " mm > " << 0.7 * layer_thickness << " mm  [ NOT OK ]\n\n";
    }
}

void Elastomeric_Bearing::Calculation::rotation_allowable(){
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    double S = Calculation::shape_factor();
    double stress_trans_sx = 0.5 * shear_mod * S * std::pow((l/ layer_thickness), 2.0) * (rotation_trans + 0.005) / n_layer; 
    double stress_long_sz = 0.5 * shear_mod * S * std::pow((w/ layer_thickness), 2.0) * (rotation_long + 0.005) / n_layer; 
    
    auto ret1 = [=](){
        if (stress_s_ >= stress_trans_sx)
            return "[ OK ]";
        else
            return "[ NOT OK ]";
    }; 
    auto ret2 = [=](){
        if (stress_s_ >= stress_long_sz)
            return "[ OK ]";
        else
            return "[ NOT OK ]";
    }; 

    myfile << "6. Cek Rotasi:\n";
    myfile << "   syarat stress_s >= 0.5 G.S (L/hri)^2  tetha_sx/ jumlah lapisan\n   " << stress_s_ << " MPa >= " << stress_trans_sx << " MPa  "<< ret1() << "\n\n";
    myfile << "   syarat stress_s >= 0.5 G.S (W/hri)^2  tetha_sz/ jumlah lapisan\n   " << stress_s_ << " MPa >= " << stress_long_sz << " MPa  "<< ret2() << "\n\n";
}

void Elastomeric_Bearing::Calculation::stability() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "7. Cek Stabilitas:\n";
    
    if(h_total <= l/3.){
        myfile << "   syarat H <= L/3\n";
        myfile << "   " << h_total << " mm <= " << l/3.0 << " mm  [ OK ]\n\n";
    }
    else{
        myfile << "   syarat H <= L/3\n";
        myfile << "   " << h_total << " mm > " << l/3.0 << " mm  [ NOT OK ]\n\n";
    }

    if(h_total <= w/3.){
        myfile << "   syarat H <= W/3\n";
        myfile << "   " << h_total << " mm <= " << w/3.0 << " mm  [ OK ]\n\n";
    }
    else{
        myfile << "   syarat H <= W/3\n";
        myfile << "   " << h_total << " mm > " << w/3.0 << " mm  [ NOT OK ]\n\n";
    }
}

void Elastomeric_Bearing::Calculation::plate_thickness() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "8. Menentukan Tebal Pelat:\n";
    myfile << "   Kondisi Layan\n";
    myfile << "   hs_min >= 3 hr_max. stress_s/ fy\n";
    myfile << "   hs_min >= " << 3. * layer_thickness * stress_s_ / fy << " mm\n";
    if(layer_steel_plate_thickness >= 3. * layer_thickness * stress_s_ / fy)
        myfile << "   " << layer_steel_plate_thickness << " mm > " << 3. * layer_thickness * stress_s_ / fy << " mm  [ OK ]\n\n";
    else
        myfile << "   " << layer_steel_plate_thickness << " mm < " << 3. * layer_thickness * stress_s_ / fy << " mm  [ NOT OK ]\n\n";

    myfile << "   Kondisi Fatik\n";
    myfile << "   hs_min >= 2 hr_max. stress_l/ fth\n";
    myfile << "   hs_min >= " << 2. * layer_thickness * stress_l_ / delta_fth << " mm\n";        
    if(layer_steel_plate_thickness >= 2. * layer_thickness * stress_l_ / delta_fth)
        myfile << "   " << layer_steel_plate_thickness << " mm > " << 2. * layer_thickness * stress_l_ / delta_fth << " mm  [ OK ]\n\n";
    else
        myfile << "   " << layer_steel_plate_thickness << " mm < " << 2. * layer_thickness * stress_l_ / delta_fth << " mm  [ NOT OK ]\n\n";
}

void Elastomeric_Bearing::Calculation::summary() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "9. Rekapitulasi Hasil Perhitungan:\n";
    myfile << "   Sifat Fisik:\n";
    myfile << "   Mutu pelat baja (fy)            = " << fy << " MPa\n";
    myfile << "   Konstanta Amplitudo Fatik (fth) = " << delta_fth << " MPa\n";
    myfile << "   Modulus Geser Elastomer (G)     = " << shear_mod << " MPa\n\n";

    myfile << "   Geometri:\n";
    myfile << "   Dimensi Bantalan/Elastomer (L x W x H) = " << w << " mm " << " x " << l << " mm x " << h_total << " mm\n";
    myfile << "   Tebal Lapisan Penutup Atas             = " << h_cover << " mm\n";
    myfile << "   Tebal Lapisan Penutup Bawah            = " << h_cover << " mm\n";
    myfile << "   Tebal Lapisan Internal                 = " << layer_thickness << " mm\n";
    myfile << "   Jumlah Lapisan Karet Internal          = " << n_layer_interior << " buah\n";
    myfile << "   Tebal Pelat Baja yang digunakan        = " << layer_steel_plate_thickness << " mm\n";
    myfile << "   Jumlah Lapisan Pelat Baja              = " << n_layer_interior + 1. << " buah\n";
    myfile << "-----------------------------------------------Finish----------------------------------------------\n";
}

void Elastomeric_Bearing::Calculation::print_data() const{
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::app);
    myfile << "Diketahui:\n";
    myfile << "   Beban Mati (DL)                          : " << p_deadload << " kN\n";
    myfile << "   Beban Hidup (LL)                         : " << p_liveload << " kN\n";
    myfile << "   Perpindahan Memanjang Jembatan           : " << delta_0 << " mm\n";
    myfile << "   Rotasi                                   : " << tetha << " rad\n";
    myfile << "   Lebar Girder (sayap gelagar)             : " << flange_width << " mm\n\n";

    myfile << "   Data Fisik Elastomer:\n";
    myfile << "   Hardness                                 : " << hardness << "\n";
    myfile << "   Modulus Geser(G)                         : " << shear_mod << " MPa\n";
    myfile << "   Total Beban Kompresi (PT)                : " << p_compressionload_unfactored << " kN\n";
    myfile << "   Batas Tegangan Delaminasi                : " << delamination_stress_limit << " MPa\n\n";
    myfile.close();
} 

void Elastomeric_Bearing::running(){
    std::ofstream myfile;
    myfile.open("output.txt", std::ios::trunc);
    myfile << "---------------------------------------------------------------------------------------------------\n";
    myfile << "|                                 PERHITUNGAN BANTALAN ELASTOMER BERLAPIS                         |\n";
    myfile << "|                               BERDASARKAN SE MENTERI PUPR NO:10/SE/M/2015                       |\n";
    myfile << "|                                      copyrights : Abdul Rochim                                  |\n";
    myfile << "---------------------------------------------------------------------------------------------------\n";

    myfile.close();

    double n = n_layer_interior + n_layer_exterior;

    Elastomeric_Bearing::Calculation calc = Elastomeric_Bearing::Calculation(dead_load, live_load, horizontal_deformation, rotation, beam_width, hardness, modulus_shear, total_compression_load, limit_of_delamination_stress, width_w, length_l, total_rubber_thickness_t, rubber_layer_thickness, cover_layer_thickness, n, fy__);
    calc.print_data();
    calc.area_elastomeric();
    calc.print_dimensionelastomeric();
    calc.shape_factor();
    calc.print_shape_factor();
    calc.stress_allowable();
    calc.deformation_shear();
    calc.rotation_allowable();
    calc.stability();
    calc.plate_thickness();
    calc.summary();

    myfile.close();
}
