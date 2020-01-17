#ifndef DATA_STORATE_HPP
#define DATA_STORATE_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace insite {

struct Spike {
  double simulation_time;
  std::uint64_t gid;
};
static_assert(sizeof(Spike) == 2 * 8);

struct MultimeterInfo {
  std::uint64_t device_id;
  std::vector<std::string> double_attributes;
  std::vector<std::string> long_attributes;
  std::vector<std::uint64_t> gids;
};

struct MultimeterMeasurements {
  std::vector<double> simulation_times;
  std::vector<std::uint64_t> gids;
  std::vector<double> values;
};

class DataStorage {
 public:
  DataStorage(const std::string& filename
              /*, hsize_t time_chunk_size, hsize_t neuronids_chunk_size*/);

  void AddSpike(double simulation_time, std::uint64_t gid);
  std::vector<Spike> GetSpikes();
  void Flush();

  void AddMultimeterMeasurement(std::uint64_t device_id, 
    const std::string& attribute_name, const double simulation_time,
    const std::uint64_t gid, const double value);
  std::unordered_map<std::uint64_t, std::unordered_map<std::string, 
    MultimeterMeasurements>> GetMultimeterMeasurements();

 private:
  // std::unique_ptr<H5::H5File> h5_file_;

  // uint64_t flushed_spikes_count = 0;
  std::vector<Spike> buffered_spikes_;
  // H5::DataSet spikes_times_dataset_;
  // H5::DataSet spikes_neurons_dataset_;
  std::mutex spike_mutex_;

  // Device ID to attribute index to measurement map.
  std::unordered_map<std::uint64_t, std::unordered_map<std::string, 
    MultimeterMeasurements>> buffered_measurements_;

  std::mutex measurement_mutex_;
};

}  // namespace insite

#endif