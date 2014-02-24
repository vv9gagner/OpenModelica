/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Linköping University,
 * Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3
 * AND THIS OSMC PUBLIC LICENSE (OSMC-PL).
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S
 * ACCEPTANCE OF THE OSMC PUBLIC LICENSE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from Linköping University, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or
 * http://www.openmodelica.org, and in the OpenModelica distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS
 * OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */


/*
 Mahder.Gebremedhin@liu.se  2014-02-10
*/




#include <pm_task_system.hpp>
#include <pm_level_scheduler.hpp>
#include <om_pm_equation.hpp>
#include <pm_graph_dump.hpp>


using namespace openmodelica::parmodelica;

int main(int argc, char** argv) {

    std::string xml_file = argv[1];
    std::cout << "Reading file: " << xml_file << std::endl;
    
    std::string eq_to_read;
    if(argc == 3) {
        eq_to_read = argv[2];
        std::cout << "Reading eqs: " << eq_to_read << std::endl;
    }
    else
        eq_to_read = "equations";
        
    
    TaskSystem<Equation> task_system;                
    task_system.load_from_xml(xml_file, eq_to_read);
    task_system.construct_graph();
    
    dump_graphml(task_system, xml_file + "_" + eq_to_read);
    
    LevelScheduler<Equation> level_scheduler(task_system);
    level_scheduler.schedule(4);
    level_scheduler.print_schedule();
       
    std::cout << utility::log_stream.str();
    std::cout << "system cost = " << task_system.total_cost << std::endl;
    std::cout << "scheduler cost = " << level_scheduler.total_parallel_cost << std::endl;
    std::cout << "Peak speedup = " << task_system.total_cost/level_scheduler.total_parallel_cost << std::endl;
    
    double new_total_cost = 0;
    BGL_FORALL_VERTICES(node,task_system.graph,TaskSystem<Equation>::Graph)
    {
        task_system.graph[node].cost = 1;
        ++new_total_cost;
    }
    
    level_scheduler.re_schedule(4);
    level_scheduler.print_schedule(std::cout);
    
    std::cout << "system cost = " << new_total_cost << std::endl;
    std::cout << "scheduler cost = " << level_scheduler.total_parallel_cost << std::endl;
    std::cout << "Peak speedup = " << new_total_cost/level_scheduler.total_parallel_cost << std::endl;
    
    // PM_functionDAE(10);
    
}